#include "Weapons/CCtrlAction.h"

UCCtrlAction::UCCtrlAction()
{
}

void UCCtrlAction::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment)
{
	OwnerCharacter = InOwner;
	Weapon = InWeapon;
	Attachment = InAttachment;
}
