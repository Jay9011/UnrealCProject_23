#include "Components/CWeaponComponent.h"

#include "CStateComponent.h"
#include "GameFramework/Character.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CCtrlAction.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponObject.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for(int32 i = 0; i < static_cast<int32>(EEquipSlotType::Max); i++)
	{
		//WeaponAsset이 존재한다면 각 WeaponAsset의 BeginPlay를 호출한다.
		if(!!WeaponAssets[i])
		{
			WeaponAssets[i]->BeginPlay(OwnerCharacter, &WeaponObject[i]);
		}
	}

#if DEBUG_WEAPON_COMPONENT
	if(UDebuggerComponent* Debugger = GetOwner()->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(UCDoSubAction* SubAction = GetSubAction())
		SubAction->Tick(DeltaTime);
}

void UCWeaponComponent::SetMode(EEquipSlotType InSlotType)
{
	//같은 슬롯 타입이라면 무기를 해제한다.
	if (Type == InSlotType)
	{
		SetUnarmedMode();
		return;
	}
	//무기가 장착되어있다면 현재 무기를 해제한다.
	else if(IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if(!!WeaponObject[static_cast<int32>(InSlotType)])
	{
		WeaponObject[static_cast<int32>(InSlotType)]->GetEquipment()->Equip();

		ChangeType(InSlotType);
	}
}

void UCWeaponComponent::ChangeType(EEquipSlotType InType)
{
	EEquipSlotType PrevType = Type;
	Type = InType;

	if(OnEquipSlotTypeChanged.IsBound())
	{
		OnEquipSlotTypeChanged.Broadcast(PrevType, Type);
	}
	if(OnWeaponTypeChanged.IsBound())
	{
		OnWeaponTypeChanged.Broadcast(WeaponAssets[static_cast<int32>(PrevType)]->GetWeaponType(), WeaponAssets[static_cast<int32>(Type)]->GetWeaponType());
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

void UCWeaponComponent::Ctrl_Pressed()
{
	if(!!GetCtrlAction())
		GetCtrlAction()->Pressed();
}

void UCWeaponComponent::Ctrl_Released()
{
	if(!!GetCtrlAction())
		GetCtrlAction()->Released();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr)

	return WeaponObject[static_cast<int32>(Type)]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr)

	return WeaponObject[static_cast<int32>(Type)]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr)
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr)

	return WeaponObject[static_cast<int32>(Type)]->GetDoAction();
}

UCDoSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr);

	return WeaponObject[static_cast<int32>(Type)]->GetDoSubAction();
}

UCCtrlAction* UCWeaponComponent::GetCtrlAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr);

	return WeaponObject[static_cast<int32>(Type)]->GetCtrlAction();
}

IIExcuteAction* UCWeaponComponent::GetCurrentAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr);

	return WeaponObject[static_cast<int32>(Type)]->GetCurrentAction();
}

IIExcuteAction* UCWeaponComponent::GetReservedAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!WeaponObject[static_cast<int32>(Type)], nullptr);

	return WeaponObject[static_cast<int32>(Type)]->GetReservedAction();
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckFalse(IsIdleMode())
	
	GetEquipment()->Unequip();
	ChangeType(EEquipSlotType::Max);
}

void UCWeaponComponent::SetMainWeaponMode()
{
	CheckFalse(IsIdleMode())
	SetMode(EEquipSlotType::MainWeapon);
}

bool UCWeaponComponent::IsIdleMode()
{
	return Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()))->IsIdleMode();
}

bool UCWeaponComponent::IsBladeMode() const
{ return WeaponAssets[static_cast<int32>(Type)]->GetWeaponType() == EWeaponType::Blade; }

EWeaponType UCWeaponComponent::GetWeaponType() const
{ return WeaponAssets[static_cast<int32>(Type)]->GetWeaponType(); }


#if DEBUG_WEAPON_COMPONENT
bool UCWeaponComponent::IsDebugEnable()
{
	if (IsUnarmedMode())
	{
		return false;
	}

	return true;
}

FDebugInfo UCWeaponComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;
	DebugInfo.Priority = static_cast<int32>(DEBUG_NUMS::WEAPON_COMPONENT);

	DebugInfo.Data.Add({"Weapon: " + StaticEnum<EWeaponType>()->GetNameStringByValue(static_cast<uint8>(GetWeaponType())), FColor::Black});

	UCDoAction* DoAction = GetDoAction();
	if (!!DoAction)
	{
		IIDebugCollector* DoActionData = Cast<IIDebugCollector>(DoAction);
		if (!!DoActionData)
		{
			if(DoActionData->IsDebugEnable())
			{
				DebugInfo.Data.Append(DoActionData->GetDebugInfo().Data);
			}
		}
	}
	
	DebugInfo.Data.Add({"Attachment: " + (GetAttachment() ? GetAttachment()->GetName() : "None"), FColor::Black});
	DebugInfo.Data.Add({"Equipment: " + (GetEquipment() ? GetEquipment()->GetName() : "None"), FColor::Black});
	// DebugInfo.Data.Add({"DoAction: " + (!!DoAction ? DoAction->GetActionName() : "None"), FColor::Black});
	// DebugInfo.Data.Add({"SubAction: " + (GetSubAction() ? GetSubAction()->GetActionName() : "None"), FColor::Black});
	DebugInfo.Data.Add({"CurrentAction: " + (GetCurrentAction() ? GetCurrentAction()->GetActionName() : "None"), FColor::Black});
	DebugInfo.Data.Add({"ReservedAction: " + (GetReservedAction() ? GetReservedAction()->GetActionName() : "None"), FColor::Black});
	
	return DebugInfo;
}
#endif