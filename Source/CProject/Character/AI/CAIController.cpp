#include "Character/AI/CAIController.h"

#include "CAIPerceptionComponentAddExpired.h"
#include "CEnemy_AI.h"
#include "CGameMode.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "Components/CNeckComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "GameMode/CBattleSystemComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Utilities/CheckMacros.h"

ACAIController::ACAIController()
{
	Perception = CreateDefaultSubobject<UCAIPerceptionComponentAddExpired>("Perception");
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	// Sight
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	Sight->SetMaxAge(SightAge);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

	PrimaryActorTick.bCanEverTick = true;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdatedDelegate);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnTargetPerceptionUpdatedDelegate);
	Perception->OnHandleExpired.AddDynamic(this, &ACAIController::OnHandleExpiredDelegate);

#if WITH_EDITOR
	if (Sight == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Sight is nullptr"));
		return;
	}
#endif
	
}

void ACAIController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	AActor* FocusActor = GetFocusActor();
	if (FocusActor == nullptr)
		return;
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID());
	CheckNeckComponent(Enemy.Get());

	CheckNull(Enemy->GetBehaviorTree());
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior = Cast<UCAIBehaviorComponent>(Enemy->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	Behavior->SetBlackboard(Blackboard);
	
	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::SetFacialFocus(ACBaseCharacter* InActor)
{
	if (InActor == nullptr)
	{
		bFocusFacial = false;
		FocusFacialActor = nullptr;
	}
	else
	{
		bFocusFacial = true;
		FocusFacialActor = InActor;
	}
}

FVector ACAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	if (Actor == nullptr)
	{
		return FAISystem::InvalidLocation;
	}

	if (bFocusFacial)
	{
		CheckNeckThreshold(Enemy.Get());
		
		return FocusFacialActor->GetHeadOffset();
	}

	return Actor->GetActorLocation();
}

bool ACAIController::CheckNeckComponent(ACharacter* Actor)
{
	UCNeckComponent* NeckComponent = Cast<UCNeckComponent>(Actor->GetComponentByClass(UCNeckComponent::StaticClass()));
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Actor->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (NeckComponent == nullptr)
	{
		if (MovementComponent != nullptr)
			MovementComponent->EnableControlRotation();

		return false;
	}

	return true;
}

void ACAIController::CheckNeckThreshold(const ACharacter* Actor) const
{
	UCNeckComponent* NeckComponent = Cast<UCNeckComponent>(Actor->GetComponentByClass(UCNeckComponent::StaticClass()));
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Actor->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (NeckComponent == nullptr || MovementComponent == nullptr)
		return;

	if (NeckComponent->CheckNearLimitYaw(NeckThreshold) && bSetControlRotationFromPawnOrientation)
	{
		MovementComponent->BackupControlRotation();
		MovementComponent->EnableControlRotation();
	}
	else
	{
		MovementComponent->RestoreControlRotation();
	}
}

bool ACAIController::UpdateCurrentTarget()
{
	TArray<AActor*> NowPerceptionActors;
	Perception->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), NowPerceptionActors);

	if (NowPerceptionActors.Num() == 0)
	{
		Blackboard->SetValueAsObject(Behavior->GetTargetKey(), nullptr);
		return false;
	}

	Blackboard->SetValueAsObject(Behavior->GetTargetKey(), NowPerceptionActors[0]);
	return true;
}

void ACAIController::SightTargetInEvent(AActor* Actor, const FAIStimulus& Stimulus)
{
	UpdateCurrentTarget();
	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), nullptr);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), FVector::ZeroVector);
}

void ACAIController::SightTargetLostEvent(AActor* Actor, const FAIStimulus& Stimulus)
{
	if (UpdateCurrentTarget())
		return;

	// 현재 타겟이 없다면
	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), Actor);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), Stimulus.StimulusLocation);

#if WITH_EDITOR
	DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 10.f, 10, FColor::Red, false, 3.f);
#endif
	
}

void ACAIController::SightTargetForgetEvent(const FAIStimulus& Stimulus)
{
	// 혹시 모를 경우를 대비해 다시 한번 타겟을 업데이트 한다.
	if (UpdateCurrentTarget())
		return;

	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), nullptr);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), FVector::ZeroVector);
}

void ACAIController::OnPerceptionUpdatedDelegate(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Blackboard);
	CheckFalse(Behavior.IsValid());
}

void ACAIController::OnTargetPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus)
{
	CheckNull(Blackboard);
	CheckFalse(Behavior.IsValid());

	ACPlayer* Player = Cast<ACPlayer>(Actor);
	if (Player == nullptr)
		return;

	// Sense가 Sight였으면
	if (Sight.IsValid() && Stimulus.Type == Sight->GetSenseID())
	{
		// 타겟이 시야에 들어온 경우
		if (Stimulus.WasSuccessfullySensed())
		{
			SightTargetInEvent(Actor, Stimulus);
		}
		// 타겟이 시야에서 벗어난 경우
		else
		{
			SightTargetLostEvent(Actor, Stimulus);
		}
	}
}

void ACAIController::OnHandleExpiredDelegate(FAIStimulus& StimulusStore)
{
	CheckNull(Blackboard);
	CheckFalse(Behavior.IsValid());

	// 만약, 만료되는 감지 정보가 Sight라면
	if (Sight.IsValid() && StimulusStore.Type == Sight->GetSenseID())
	{
		SightTargetForgetEvent(StimulusStore);
	}
	
}

UCAIBehaviorComponent* ACAIController::GetBehaviorComponent() const
{
	CheckFalseResult(Behavior.IsValid(), nullptr);

	return Behavior.Get();
}
