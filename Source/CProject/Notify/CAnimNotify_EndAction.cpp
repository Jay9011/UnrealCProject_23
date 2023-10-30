#include "Notify/CAnimNotify_EndAction.h"

#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = Cast<UCWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNull(weapon);
	CheckNull(weapon->GetCurrentAction());

	IIExcuteAction* action = Cast<IIExcuteAction>(weapon->GetCurrentAction());
	CheckNull(action);

	action->End_Action();
}
