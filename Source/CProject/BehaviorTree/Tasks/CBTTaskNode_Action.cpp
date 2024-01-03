#include "BehaviorTree/Tasks/CBTTaskNode_Action.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = TEXT("Action");

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(Enemy->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(Weapon, EBTNodeResult::Failed);

	if(!Weapon->GetEquipment()->Data.bCanMove)
		Controller->StopMovement();
	
	Weapon->DoAction();
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(Enemy->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));

	bool bCheck = true;
	bCheck &= (State->IsIdleMode());
	bCheck &= (Weapon->GetDoAction()->IsInAction() == false);

	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}
