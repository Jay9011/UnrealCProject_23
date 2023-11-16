#include "Weapons/DoActions/CDoAction_Combo.h"

#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

void UCDoAction_Combo::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas)
{
	Super::BeginPlay(InOwnerWeaponAsset, InAttachment, InEquipment, InOwner, InDoActionDatas);
	ComboState = NewObject<UCComboState>(this);
}

void UCDoAction_Combo::DoAction()
{
	CheckFalse(DoActionDatas.IsValidIndex(ComboState->GetIndex()))

	// 만약 콤보를 예약할 수 있는 상태라면
	if (ComboState->IsEnable()
		&& DoActionDatas.IsValidIndex(ComboState->GetIndex() + 1))
	{
		ComboState->DisableCombo();
		ComboState->ReserveCombo();
		OwnerWeaponAsset->SetCurrentAction(this);

		return;
	}

	CheckFalse(StateComponent->IsIdleMode())
	//만약 제일 위쪽에 Super::DoAction();이 있었다면, 부모의 StateComponent->SetActionMode();가 호출되어 위쪽 검사를 통과하지 못하고 여기서 멈추게 된다.  
	Super::DoAction();

	DoActionDatas[ComboState->GetIndex()].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(ComboState->IsExist())

	ComboState->ExpireCombo();
	DoActionDatas[ComboState->IncreaseCombo()].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	ComboState->ResetCombo();
}

void UCDoAction_Combo::Begin_Action()
{
	Begin_DoAction();
}

void UCDoAction_Combo::End_Action()
{
	End_DoAction();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);
	CheckTrue(DoActionDatas.Num() <= ComboState->GetIndex());
	
	DoActionDatas[ComboState->GetIndex()].DamagedData.SendDamage(InAttacker, InAttackCauser, InOther);
}

#if WITH_EDITOR
bool UCDoAction_Combo::IsDebugEnable()
{
	return !!ComboState;
}

FDebugInfo UCDoAction_Combo::GetDebugInfo()
{
	FDebugInfo DebugInfo;

	DebugInfo.Data.Add({"Index : " + FString::FromInt(ComboState->GetIndex()), FColor::Red});
	DebugInfo.Data.Add({"Combo Enable : " + FString(ComboState->IsEnable() ? "true" : "false"), FColor::Red});
	DebugInfo.Data.Add({"Combo Exist : " + FString(ComboState->IsExist() ? "true" : "false"), FColor::Red});

	return DebugInfo;
}
#endif
