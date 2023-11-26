#include "Weapons/DoSubActions/CDoSubAction_HeavyAttack.h"

#include "Components/CStateComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

void UCDoSubAction_HeavyAttack::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwnerWeaponAsset, InOwner, InAttachment, InDoAction);

	InitDoActionData();

	DoAction_Combo = Cast<UCDoAction_Combo>(InDoAction);
	CheckNull(DoAction_Combo);

	ComboState = DoAction_Combo->GetComboState();
}

void UCDoSubAction_HeavyAttack::Pressed()
{
	// CheckTrue(StateComponent->IsSubActionMode()); // 서브액션 도중이면 실행 무시
	CheckFalse(HeavyAttackDatas.IsValidIndex(ComboState->GetIndex()));

	// 콤보를 예약할 수 있는 상태면서 다음 강공격 콤보가 존재한다면 강공격 콤보를 예약
	if(ComboState->IsEnable()
		&& HeavyAttackDatas.IsValidIndex(ComboState->GetIndex() + 1))
	{
		ComboState->DisableCombo();
		ComboState->ReserveCombo();
		OwnerWeaponAsset->ReserveAction(this);

		return;
	}

	// 첫 실행인 경우만 통과 (IdleMode 일때만)
	CheckFalse(StateComponent->IsIdleMode());
	Super::Pressed();

	StateComponent->SetActionMode();
	StateComponent->OnSubActionMode();
	OwnerWeaponAsset->SetCurrentAction(this);

	HeavyAttackDatas[ComboState->GetIndex()].DoAction(OwnerCharacter);
}

void UCDoSubAction_HeavyAttack::BeginSubAction_Implementation()
{
	Super::BeginSubAction_Implementation();

	ComboState->ExpireCombo();
	StateComponent->OnSubActionMode();
	OwnerWeaponAsset->UpdateActions();
	HeavyAttackDatas[ComboState->IncreaseCombo()].DoAction(OwnerCharacter);
}

void UCDoSubAction_HeavyAttack::EndSubAction_Implementation()
{
	Super::EndSubAction_Implementation();

	DoAction_Combo->End_Action();
	StateComponent->OffSubActionMode();
	OwnerWeaponAsset->UpdateActions();
}

void UCDoSubAction_HeavyAttack::Begin_Action()
{
	CheckFalse(ComboState->IsExist());
	BeginSubAction_Implementation();
}

void UCDoSubAction_HeavyAttack::End_Action()
{
	EndSubAction_Implementation();
}

void UCDoSubAction_HeavyAttack::ChangingProcess()
{
	StateComponent->OffSubActionMode();
}

void UCDoSubAction_HeavyAttack::InitDoActionData()
{
	if (!!HeavyAttackDataTable)
	{
		TArray<FDoActionData*> DoActionData;
		HeavyAttackDataTable->GetAllRows<FDoActionData>("", DoActionData);

		for (FDoActionData* Data : DoActionData)
			HeavyAttackDatas.Add(*Data);
	}
	else if (HeavyAttackDatas.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("HeavyAttackDataTable is nullptr"));
	}
}
