#include "BehaviorTree/Tasks/CBTTaskNode_Hitted.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Utilities/CheckMacros.h"

UCBTTaskNode_Hitted::UCBTTaskNode_Hitted()
{
	NodeName = "Hitted";
	
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNullResult(Controller, EBTNodeResult::Failed);

	Controller->StopMovement();
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNull(Controller);

	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	CheckNull(AI);

	UCStateComponent* State = Cast<UCStateComponent>(AI->GetComponentByClass(UCStateComponent::StaticClass()));

	if (State->IsHittedMode() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}
