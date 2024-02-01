#include "Character/AI/CAIController.h"

#include "CAIPerceptionComponentAddExpired.h"
#include "CEnemy_AI.h"
#include "CGameMode.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "Components/CNeckComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Utilities/CheckMacros.h"

ACAIController::ACAIController()
{
	Perception = CreateDefaultSubobject<UCAIPerceptionComponentAddExpired>("Perception");
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	// Sight
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;

		Perception->ConfigureSense(*Sight);
		Perception->SetDominantSense(*Sight->GetSenseImplementation());
	}

	// Hearing
	{
		Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing");

		Hearing->DetectionByAffiliation.bDetectEnemies = true;
		Hearing->DetectionByAffiliation.bDetectFriendlies = false;
		Hearing->DetectionByAffiliation.bDetectNeutrals = false;

		Perception->ConfigureSense(*Hearing);
		Perception->SetDominantSense(*Hearing->GetSenseImplementation());
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnTargetPerceptionUpdatedDelegate);
	Perception->OnHandleExpired.AddDynamic(this, &ACAIController::OnHandleExpiredDelegate);

#if WITH_EDITOR
	if (Sight == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Sight is nullptr"));
	}
	if (Hearing == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Hearing is nullptr"));
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
	Blackboard->InitializeBlackboard(*Enemy->GetBehaviorTree()->BlackboardAsset);
	
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

void ACAIController::SightTargetInEvent(AActor* Actor, FAIStimulus& Stimulus)
{
	UpdateCurrentTarget();
	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), nullptr);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), FVector::ZeroVector);
}

void ACAIController::SightTargetLostEvent(AActor* Actor, FAIStimulus& Stimulus)
{
	if (UpdateCurrentTarget())
		return;

	// 현재 타겟이 없다면
	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), Actor);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), Stimulus.StimulusLocation);
	// 처음 움직이는 위치는 타겟을 잃어버린 위치로 한다.
	Blackboard->SetValueAsVector(Behavior->GetMoveToLocationKey(), Stimulus.StimulusLocation);
}

void ACAIController::SightTargetForgetEvent(FAIStimulus& Stimulus)
{
	// 혹시 모를 경우를 대비해 다시 한번 타겟을 업데이트 한다.
	if (UpdateCurrentTarget())
		return;

	Blackboard->SetValueAsObject(Behavior->GetLostTargetKey(), nullptr);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), FVector::ZeroVector);
}

void ACAIController::HearingEvent(AActor* Actor, FAIStimulus& Stimulus)
{
	// 새로운 듣기라면
	if (Blackboard->GetValueAsInt(HearingBlackboardKey) == 0)
	{
		Blackboard->SetValueAsInt(HearingBlackboardKey, 1);
		Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), Stimulus.StimulusLocation);
		return;
	}
	// 듣기 중에 듣기가 감지된 경우
	if (Blackboard->GetValueAsInt(HearingBlackboardKey) == 1)
	{
		Blackboard->SetValueAsInt(HearingBlackboardKey, 2);
		Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), Stimulus.StimulusLocation);
		Blackboard->SetValueAsVector(Behavior->GetMoveToLocationKey(), Stimulus.StimulusLocation);
		return;
	}
}

void ACAIController::VigilanceHearingEvent(AActor* Actor, FAIStimulus& Stimulus)
{
	Blackboard->SetValueAsInt(HearingBlackboardKey, 1);
	Blackboard->SetValueAsVector(Behavior->GetLostTargetLocationKey(), Stimulus.StimulusLocation);
}

void ACAIController::OnTargetPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus)
{
	CheckNull(Blackboard);
	CheckFalse(Behavior.IsValid());

	ACPlayer* Player = Cast<ACPlayer>(Actor);
	if (Player == nullptr)
		return;

	// Sense가 Sight였으면
	if (Sight != nullptr && Stimulus.Type == Sight->GetSenseID())
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
	// Sense가 Hearing이었다면,
	else if (Hearing != nullptr && Stimulus.Type == Hearing->GetSenseID() && Stimulus.WasSuccessfullySensed())
	{
		EAIStateType AIStateType = Behavior->GetAIStateType();
		// 만약, 순찰 중이었다면
		if (Behavior->IsPatrol())
		{
			HearingEvent(Actor, Stimulus);
		}
	}
}

void ACAIController::OnHandleExpiredDelegate(FAIStimulus& StimulusStore)
{
	CheckNull(Blackboard);
	CheckFalse(Behavior.IsValid());

	// 만약, 만료되는 감지 정보가 Sight라면
	if (Sight != nullptr && StimulusStore.Type == Sight->GetSenseID())
	{
		SightTargetForgetEvent(StimulusStore);
	}
	// 만약, 만료되는 감지 정보가 Hearing 이라면
	else if (Hearing != nullptr && StimulusStore.Type == Hearing->GetSenseID())
	{
		Blackboard->SetValueAsInt(HearingBlackboardKey, 0);
	}
	
}

UCAIBehaviorComponent* ACAIController::GetBehaviorComponent() const
{
	CheckFalseResult(Behavior.IsValid(), nullptr);

	return Behavior.Get();
}
