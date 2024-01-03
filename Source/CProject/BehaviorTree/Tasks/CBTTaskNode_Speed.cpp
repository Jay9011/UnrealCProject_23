#include "BehaviorTree/Tasks/CBTTaskNode_Speed.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	UCMovementComponent* Movement = Cast<UCMovementComponent>(Enemy_AI->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (Movement == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	Movement->SetSpeed(SpeedType);

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UCBTTaskNode_Speed::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBTTaskNode_Speed::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UCBTTaskNode_Speed::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
