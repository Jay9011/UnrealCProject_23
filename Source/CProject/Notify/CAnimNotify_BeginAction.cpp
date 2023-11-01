#include "Notify/CAnimNotify_BeginAction.h"

#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "Begin_DoAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentAction());

	action = Cast<IIExcuteAction>(weapon->GetCurrentAction());
	CheckNull(action);

	action->Begin_Action();
}