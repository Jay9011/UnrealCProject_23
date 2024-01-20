#include "BehaviorTree/Tasks/CBTTask_MoveTo_WithAbort.h"

#include "Character/AI/CAIController.h"

EBTNodeResult::Type UCBTTask_MoveTo_WithAbort::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (AIController != nullptr)
	{
		AIController->StopMovement();
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}
