#include "BehaviorTree/Tasks/CBTTask_SetBlackboard_Int.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Utilities/CheckMacros.h"

UCBTTask_SetBlackboard_Int::UCBTTask_SetBlackboard_Int()
{
	NodeName = TEXT("Set Int");
}

EBTNodeResult::Type UCBTTask_SetBlackboard_Int::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	CheckNullResult(Blackboard, EBTNodeResult::Failed);

	Blackboard->SetValueAsInt(GetSelectedBlackboardKey(), Value);

	return EBTNodeResult::Succeeded;
}
