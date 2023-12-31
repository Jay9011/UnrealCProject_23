#include "Weapons/CtrlActions/GuardActions/CParryAction.h"

#include "Character/CBaseCharacter.h"
#include "Components/CMovementComponent.h"

void UCParryAction::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon)
{
	Super::BeginPlay(InOwner, InWeapon);
	
	OwnerBaseCharacter = Cast<ACBaseCharacter>(InOwner);
}

void UCParryAction::End_DoAction()
{
	Super::End_DoAction();
}

void UCParryAction::End_Action()
{
	Super::End_Action();

	Attacker = nullptr;
}

void UCParryAction::DoAction(ACharacter* InAttacker)
{
	Attacker = InAttacker;
}
