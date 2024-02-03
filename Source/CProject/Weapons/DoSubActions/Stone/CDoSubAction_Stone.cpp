#include "Weapons/DoSubActions/Stone/CDoSubAction_Stone.h"

#include "Components/CPredictionPathComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/DoActions/CDoAction_Stone.h"

void UCDoSubAction_Stone::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment,
                                    UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment, InDoAction);

	PredictionPath = Cast<UCPredictionPathComponent>(InOwner->GetComponentByClass(UCPredictionPathComponent::StaticClass()));
	StoneAction = Cast<UCDoAction_Stone>(InDoAction);
}

void UCDoSubAction_Stone::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	if (StateComponent->IsIdleMode()
		&& PredictionPath.IsValid()
		&& bPressed
		&& StoneAction->ReadyProjectileCheck()
		)
	{
		FPredictProjectilePathParams Params
		(
			StoneAction->GetProjectileRadius(),
			StoneAction->GetSocketLocation(),
			StoneAction->GetDirection() * StoneAction->GetProjectileSpeed(),
			2.f,
			ECollisionChannel::ECC_WorldDynamic,
			OwnerCharacter
		);
		Params.SimFrequency = 15.f;
		
		PredictionPath->ShowAndUpdate(Params);
	}
	else
	{
		PredictionPath->Hide();
	}
}

void UCDoSubAction_Stone::Pressed()
{
	Super::Pressed();
}

void UCDoSubAction_Stone::Released()
{
	Super::Released();
	PredictionPath->Hide();
}
