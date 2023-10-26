#include "Notify/CAnimNotifyState_SubAction.h"

#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CDoSubAction.h"

FString UCAnimNotifyState_SubAction::GetNotifyName_Implementation() const
{
	return "SubAction";
}

void UCAnimNotifyState_SubAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(WeaponComponent);
	CheckNull(WeaponComponent->GetSubAction());

	WeaponComponent->GetSubAction()->BeginSubAction();
}

void UCAnimNotifyState_SubAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(WeaponComponent);
	CheckNull(WeaponComponent->GetSubAction());

	WeaponComponent->GetSubAction()->EndSubAction();
}
