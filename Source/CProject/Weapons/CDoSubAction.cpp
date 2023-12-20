#include "Weapons/CDoSubAction.h"

#include "CWeaponObject.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

UCDoSubAction::UCDoSubAction()
{
}

void UCDoSubAction::BeginPlay(ACharacter* InOwner, class UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	OwnerCharacter = InOwner;
	Weapon = InWeapon;
	Attachment = InAttachment;
	DoAction = InDoAction;

	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
}

void UCDoSubAction::Begin_Action()
{
}

void UCDoSubAction::End_Action()
{
}
