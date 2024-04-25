#include "AddOn/CAddOnComponent.h"

void UCAddOnComponent::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon)
{
	OwnerCharacter = InOwner;
	Weapon = InWeapon;
}
