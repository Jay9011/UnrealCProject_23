#include "Weapons/CWeaponStructures.h"

#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"

void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!MovementComponent)
	{
		if(bFixedCamera)
		{
			MovementComponent->EnableFixedCamera();
		}

		if(!bCanMove)
		{
			MovementComponent->Stop();
		}
	}// if (!!MovementComponent)

	if(!!ActionMontage)
	{
		InOwner->PlayAnimMontage(ActionMontage, PlayRate);
	}// if(!!ActionMontage)
}
