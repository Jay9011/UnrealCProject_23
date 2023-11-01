#include "Notify/CAnimNotifyState_Combo.h"

#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentAction());

	action = Cast<IIExcuteAction>(weapon->GetCurrentAction());
	CheckNull(action);
	
	comboState = Cast<IIComboState>(action);
	CheckNull(comboState);
	combo = comboState->GetComboState();
	CheckNull(combo);
	
	//모든 조건이 충족되었으므로 콤보를 활성화한다.
	combo->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	action = Cast<IIExcuteAction>(weapon->GetCurrentAction());
	CheckNull(action);
	
	comboState = Cast<IIComboState>(action);
	CheckNull(comboState);
	combo = comboState->GetComboState();
	CheckNull(combo);

	//모든 조건이 충족되었으므로 콤보를 비활성화한다.
	combo->DisableCombo();
}