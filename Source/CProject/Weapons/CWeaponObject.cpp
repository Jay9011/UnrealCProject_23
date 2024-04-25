#include "Weapons/CWeaponObject.h"

void UCWeaponObject::RegisterAddOnComponent(UCAddOnComponent* InAddOnComponent)
{
	AddOnComponents.Add(InAddOnComponent);
}

void UCWeaponObject::UpdateActions()
{
	SetCurrentAction(ReservedAction);
	ReservedAction = nullptr;
}
