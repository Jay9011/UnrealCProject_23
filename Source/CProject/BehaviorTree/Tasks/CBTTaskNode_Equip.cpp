#include "BehaviorTree/Tasks/CBTTaskNode_Equip.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNullResult(Controller, EBTNodeResult::Failed);
	
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	CheckNullResult(AI, EBTNodeResult::Failed);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Failed);

	// 해당 슬롯에 장비가 있는지 확인
	if (!WeaponComponent->CheckOwnEquipSlot(EquipSlotType))
	{
		return EBTNodeResult::Failed;
	}

	if (EquipSlotType == WeaponComponent->GetType())
	{
		return EBTNodeResult::Succeeded;
	}

	switch (EquipSlotType)
	{
	case EEquipSlotType::MainWeapon:
		WeaponComponent->SetMainWeaponMode();
		break;
	case EEquipSlotType::SubWeapon:
		WeaponComponent->SetSubWeaponMode();
		break;
	case EEquipSlotType::Max:
		WeaponComponent->SetUnarmedMode();
		break;
	}

	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(AI->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (MovementComponent != nullptr)
	{
		Controller->StopMovement();
	}

	// 장착이 완료되었는지 Tick에서 확인
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNull(Controller);

	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	CheckNull(AI);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(WeaponComponent);

	UCStateComponent* StateComponent = Cast<UCStateComponent>(AI->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(StateComponent);

	const bool bEquipped = WeaponComponent->GetEquipment()->IsEquipped();
	if (bEquipped && StateComponent->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Aborted);

	bool bBeginEquip = WeaponComponent->GetEquipment()->IsBeginEquip();
	if (bBeginEquip == false)
	{
		WeaponComponent->GetEquipment()->Begin_Equip();
	}

	WeaponComponent->GetEquipment()->End_Equip();
	
	return EBTNodeResult::Aborted;
}
