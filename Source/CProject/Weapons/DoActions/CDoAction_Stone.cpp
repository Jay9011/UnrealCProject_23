#include "Weapons/DoActions/CDoAction_Stone.h"

#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/Skills/Stone/CSkill_Stone.h"

void UCDoAction_Stone::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon)
{
	Super::BeginPlay(InOwner, InWeapon);
	SubAction = InWeapon->GetDoSubAction();
}

void UCDoAction_Stone::DoAction_Implementation()
{
	CheckFalse(DoActionDatas.Num() > 0);
	CheckFalse(StateComponent->IsIdleMode());
	CheckFalse(SubAction != nullptr && SubAction->IsPressed());

	Super::DoAction_Implementation();
	Weapon->ReserveAction(this);

	SavedDirection = GetDirection();

	if (SubAction.IsValid())
	{
		SubAction->Released();		
	}
	
	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_Stone::Begin_Action()
{
	Super::Begin_Action();
	
	if (ReadyProjectileCheck())
	{
		ReadyProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ReadyProjectile->ActiveSkill(SavedDirection);
		
		if (SubAction.IsValid() && SubAction->IsPressed())
		{
			SubAction->Released();
		}

		ReadyProjectile = nullptr;
	}
	
	Begin_DoAction();
}

void UCDoAction_Stone::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void UCDoAction_Stone::End_Action()
{
	Super::End_Action();
	End_DoAction();
}

void UCDoAction_Stone::End_DoAction()
{
	Super::End_DoAction();
	CreateStone();
}

void UCDoAction_Stone::OnEndEquip()
{
	Super::OnEndEquip();
	CreateStone();
}

void UCDoAction_Stone::OnUnEquip()
{
	Super::OnUnEquip();

	if (ReadyProjectileCheck())
	{
		ReadyProjectile->Destroy();
		ReadyProjectile = nullptr;
	}
}

bool UCDoAction_Stone::CreateStone()
{
	if (World->bIsTearingDown)
		return false;

	ACSkill_Stone* Stone = World->SpawnActorDeferred<ACSkill_Stone>(ProjectileClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNullResult(Stone, false);

	Stone->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	ReadyProjectile = Stone;

	Stone->FinishSpawning(FTransform::Identity);

	return true;
}

bool UCDoAction_Stone::ReadyProjectileCheck() const
{
	return ReadyProjectile && !ReadyProjectile->IsPendingKill() && ReadyProjectile->GetAttachParentActor();
}

FVector UCDoAction_Stone::GetDirection() const
{
	FVector Direction = OwnerCharacter->GetController()->GetControlRotation().Vector();
	Direction.Z = OwnerCharacter->GetMesh()->GetSocketQuaternion(SocketName).GetForwardVector().Z;

	return Direction;
}

float UCDoAction_Stone::GetProjectileSpeed() const
{
	if (ReadyProjectileCheck())
	{
		return ReadyProjectile->GetProjectileSpeed();
	}
	else
	{
		return 0.f;
	}
}

float UCDoAction_Stone::GetProjectileRadius() const
{
	return ReadyProjectileCheck() ? ReadyProjectile->GetSphereRadius() : 0.f;
}

FVector UCDoAction_Stone::GetSocketLocation() const
{
	return OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
}
