#include "Character/AI/CAIController.h"

#include "CEnemy_AI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CNeckComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Utilities/CheckMacros.h"

ACAIController::ACAIController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("Blackboard");

	// Sight
	{
		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = SightRadius;
		Sight->LoseSightRadius = LoseSightRadius;
		Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
		Sight->SetMaxAge(SightAge);

		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;
	}

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdatedDelegate);
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

	if (NeckComponent->CheckNearLimitYaw(NeckThreshold))
	{
		MovementComponent->EnableControlRotation();
	}
	else
	{
		MovementComponent->DisableControlRotation();
	}
}

void ACAIController::OnPerceptionUpdatedDelegate(const TArray<AActor*>& UpdatedActors)
{
	// 감지 된 액터가 없는 경우
	if (UpdatedActors.Num() == 0)
	{
		Blackboard->SetValueAsObject(Behavior->GetTargetKey(), nullptr);
		
		return;
	}

	Blackboard->SetValueAsObject(Behavior->GetTargetKey(), UpdatedActors[0]);
	
}

UCAIBehaviorComponent* ACAIController::GetBehaviorComponent() const
{
	CheckFalseResult(Behavior.IsValid(), nullptr);

	return Behavior.Get();
}
