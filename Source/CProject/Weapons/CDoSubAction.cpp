#include "Weapons/CDoSubAction.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

UCDoSubAction::UCDoSubAction()
{
}

void UCDoSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	StateComponent = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
}

