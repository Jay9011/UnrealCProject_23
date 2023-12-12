#include "Weapons/CDoSubAction.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

UCDoSubAction::UCDoSubAction()
{
}

void UCDoSubAction::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	OwnerWeaponAsset = InOwnerWeaponAsset;
	OwnerCharacter = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
}