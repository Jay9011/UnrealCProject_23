#include "BehaviorTree/Services/CBTServiceMelee.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/AI/CAIBehaviorComponent.h"

UCBTServiceMelee::UCBTServiceMelee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UCBTServiceMelee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	
	UCStateComponent* State = Cast<UCStateComponent>(Enemy_AI->GetComponentByClass(UCStateComponent::StaticClass()));
	UCAIBehaviorComponent* Behavior = Cast<UCAIBehaviorComponent>(Enemy_AI->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (State->IsHittedMode())
	{
		Behavior->SetHittedMode();

		return;
	}

	ACharacter* Target = Behavior->GetTarget();
	if (Target == nullptr)
	{
		Behavior->SetPatrolMode();

		return;
	}

	float Distance = Enemy_AI->GetDistanceTo(Target);
	if (Distance < MeleeRange)
	{
		Behavior->SetActionMode();

		return;
	}

	Behavior->SetApproachMode();
}
