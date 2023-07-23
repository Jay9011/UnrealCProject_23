#include "Components/CWeaponComponent.h"

#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for(int32 i = 0; i < static_cast<int32>(EWeaponType::Max); i++)
	{
		//WeaponAsset이 존재한다면 각 WeaponAsset의 BeginPlay를 호출한다.
		if(!!WeaponAsset[i])
		{
			WeaponAsset[i]->BeginPlay(OwnerCharacter);
		}
	}
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	//같은 타입이라면 무기를 해제한다.
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	//무기가 장착되어있다면 현재 무기를 해제한다.
	else if(IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if(!!WeaponAsset[static_cast<int32>(InType)])
	{
		WeaponAsset[static_cast<int32>(InType)]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType PrevType = Type;
	Type = InType;

	if(OnWeaponTypeChanged.IsBound())
	{
		OnWeaponTypeChanged.Broadcast(PrevType, Type);
	}
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponAsset[static_cast<int32>(Type)], nullptr)

	return WeaponAsset[static_cast<int32>(Type)]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponAsset[static_cast<int32>(Type)], nullptr)

	return WeaponAsset[static_cast<int32>(Type)]->GetEquipment();
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(IsIdleMode())
	
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetBladeMode()
{
	CheckFalse(IsIdleMode())
	SetMode(EWeaponType::Blade);
}

bool UCWeaponComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}