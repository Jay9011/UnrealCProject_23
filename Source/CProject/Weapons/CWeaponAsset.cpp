#include "Weapons/CWeaponAsset.h"

#include "CAttachment.h"
#include "CDoAction.h"
#include "CDoSubAction.h"
#include "CEquipment.h"
#include "CEvadeAction.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner)
{
	if(!!AttachmentClass)
	{
		FActorSpawnParameters Parameters;
		Parameters.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, Parameters);
	}// if(!!AttachmentClass)

	if(!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

		if(!!Attachment)
		{
			//Attachment가 있다면 Equip관련 이벤트에 Attachmnet의 함수를 바인딩
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}// if(!!EquipmentClass)

	if(!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas);
	}// if(!!DoActionClass)

	if(!!DoSubAction)
	{
		DoSubAction = NewObject<UCDoSubAction>(this, DoSubActionClass);
		DoSubAction->BeginPlay(InOwner, Attachment, DoAction);
	}// if(!!DoSubAction)

	if(!!EvadeActionClass)
	{
		EvadeAction = NewObject<UCEvadeAction>(this, EvadeActionClass);
		EvadeAction->BeginPlay(InOwner, EvadeData);

		if(!!Equipment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(EvadeAction, &UCEvadeAction::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(EvadeAction, &UCEvadeAction::OnUnequip);
		}
	}// if(!!EvadeActionClass)
}
