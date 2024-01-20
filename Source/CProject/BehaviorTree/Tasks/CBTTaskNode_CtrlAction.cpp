#include "BehaviorTree/Tasks/CBTTaskNode_CtrlAction.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CGuardComponent.h"
#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"

UCBTTaskNode_CtrlAction::UCBTTaskNode_CtrlAction()
{
	NodeName = "Ctrl Action";
}

EBTNodeResult::Type UCBTTaskNode_CtrlAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(AIController->GetPawn());
	CheckNullResult(AIController, EBTNodeResult::Failed);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(Enemy_AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Failed);
	
	if (bDoPressCtrl)
	{
		WeaponComponent->Ctrl_Pressed();
	}
	else
	{
		WeaponComponent->Ctrl_Released();
	}
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UCBTTaskNode_CtrlAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(AIController->GetPawn());
	CheckNullResult(AIController, EBTNodeResult::Failed);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(Enemy_AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Failed);

	WeaponComponent->Ctrl_Released();
	
	return EBTNodeResult::Aborted;
}
