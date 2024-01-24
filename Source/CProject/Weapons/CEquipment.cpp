#include "Weapons/CEquipment.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));
	StateComponent = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip_Implementation()
{
	StateComponent->SetEquipMode();

	if(Data.bCanMove == false)
	{
		MovementComponent->Stop();
	}

	if(!!Data.EquipMontage)
	{
		OwnerCharacter->PlayAnimMontage(Data.EquipMontage, Data.PlayRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if(Data.bUseControlRotation)
	{
		MovementComponent->EnableControlRotation();
	}
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if(OnEquipmentBeginEquip.IsBound())
	{
		OnEquipmentBeginEquip.Broadcast();
	}
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	MovementComponent->Move();
	StateComponent->SetIdleMode();

	if (OnEquipmentEndEquip.IsBound())
	{
		OnEquipmentEndEquip.Broadcast();
	}
}

void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;
	MovementComponent->DisableControlRotation();

	if(OnEquipmentUnEquip.IsBound())
	{
		OnEquipmentUnEquip.Broadcast();
	}
}
