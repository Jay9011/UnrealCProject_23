#include "Notify/CAnimNotifyState_Equip.h"

#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CEquipment.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp)
	CheckNull(MeshComp->GetOwner())

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon)
	CheckNull(weapon->GetEquipment())

	//UCWeaponComponent의 Begin_Equip을 호출한다. Begin_Euip에는 OnEquipmentBeginEquip 델리게이트가 브로드캐스트된다.
	weapon->GetEquipment()->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp)
	CheckNull(MeshComp->GetOwner())

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon)
	CheckNull(weapon->GetEquipment())

	//UCWeaponComponent의 End_Equip을 호출한다.
	weapon->GetEquipment()->End_Equip();
}