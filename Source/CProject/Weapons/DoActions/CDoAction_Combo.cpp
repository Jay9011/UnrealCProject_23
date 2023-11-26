#include "Weapons/DoActions/CDoAction_Combo.h"

#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

void UCDoAction_Combo::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner)
{
	// 최상위 UCDoAction_Combo의 ComboState를 공유하기 위해 Super::BeginPlay()보다 먼저 호출한다.
	if (!ComboState)
		ComboState = NewObject<UCComboState>(this);
	
	Super::BeginPlay(InOwnerWeaponAsset, InOwner);
}

void UCDoAction_Combo::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, UCComboState* InComboState)
{
	if (InComboState == nullptr)
	{
		BeginPlay(InOwnerWeaponAsset, InOwner);

		return;
	}
	
	Super::BeginPlay(InOwnerWeaponAsset, InOwner);
	ComboState = InComboState;
}

void UCDoAction_Combo::DoAction_Implementation()
{
	CheckFalse(DoActionDatas.IsValidIndex(ComboState->GetIndex()))

	// 만약 콤보를 예약할 수 있는 상태라면
	if (ComboState->IsEnable()
		&& DoActionDatas.IsValidIndex(ComboState->GetIndex() + 1))
	{
		ComboState->DisableCombo();
		ComboState->ReserveCombo();
		OwnerWeaponAsset->ReserveAction(this);

		return;
	}

	CheckFalse(StateComponent->IsIdleMode())
	//만약 제일 위쪽에 Super::DoAction();이 있었다면, 부모의 StateComponent->SetActionMode();가 호출되어 위쪽 검사를 통과하지 못하고 여기서 멈추게 된다.  
	Super::DoAction_Implementation();

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

UCDoAction_Combo* UCDoAction_Combo::BP_AddDoAction_Combo(TSubclassOf<UCDoAction_Combo> InDoActionClass, UCComboState* InComboState)
{
	UCDoAction_Combo* NewDoAction = NewObject<UCDoAction_Combo>(this, InDoActionClass);
	NewDoAction->BeginPlay(OwnerWeaponAsset, OwnerCharacter, InComboState);

	return NewDoAction;
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckFalse(OwnerWeaponAsset->GetCurrentAction() == this);
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
