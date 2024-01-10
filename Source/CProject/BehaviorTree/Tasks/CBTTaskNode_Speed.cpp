#include "BehaviorTree/Tasks/CBTTaskNode_Speed.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Utilities/CheckMacros.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCMovementComponent* Movement = Cast<UCMovementComponent>(Enemy_AI->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNullResult(Movement, EBTNodeResult::Failed);

	Movement->SetSpeed(SpeedType);

	return EBTNodeResult::Succeeded;
}