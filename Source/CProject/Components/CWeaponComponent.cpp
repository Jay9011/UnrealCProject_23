#include "Components/CWeaponComponent.h"

#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CDoSubAction.h"
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
		if(!!WeaponAssets[i])
		{
			WeaponAssets[i]->BeginPlay(OwnerCharacter);
		}
	}
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!!GetSubAction())
		GetSubAction()->Tick(DeltaTime);
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

	if(!!WeaponAssets[static_cast<int32>(InType)])
	{
		WeaponAssets[static_cast<int32>(InType)]->GetEquipment()->Equip();

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

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
	{
		GetDoAction()->DoAction();
	}
}

void UCWeaponComponent::SubAction_Pressed()
{
	if(!!GetSubAction())
		GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if(!!GetSubAction())
		GetSubAction()->Released();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponAssets[static_cast<int32>(Type)], nullptr)

	return WeaponAssets[static_cast<int32>(Type)]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponAssets[static_cast<int32>(Type)], nullptr)

	return WeaponAssets[static_cast<int32>(Type)]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponAssets[static_cast<int32>(Type)], nullptr)

	return WeaponAssets[static_cast<int32>(Type)]->GetDoAction();
}

UCDoSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!WeaponAssets[(int32)Type], nullptr);

	return WeaponAssets[(int32)Type]->GetDoSubAction();
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
