#include "Weapons/CWeaponObject.h"

void UCWeaponObject::UpdateActions()
{
	SetCurrentAction(ReservedAction);
	ReservedAction = nullptr;
}
