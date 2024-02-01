#include "BehaviorTree/Tasks/CBTTaskNode_Stop.h"

#include "Character/AI/CAIController.h"
#include "Utilities/CheckMacros.h"

UCBTTaskNode_Stop::UCBTTaskNode_Stop()
{
	NodeName = "Stop";
}

EBTNodeResult::Type UCBTTaskNode_Stop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(AIController, EBTNodeResult::Failed);

	AIController->StopMovement();

	return EBTNodeResult::Succeeded;
}