#include "Weapons/CWeaponAsset.h"

#include "CAttachment.h"
#include "CCtrlAction.h"
#include "CDoAction.h"
#include "CDoSubAction.h"
#include "CEquipment.h"
#include "CEvadeAction.h"
#include "CWeaponObject.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, class UCWeaponObject** OutWeaponObject)
{
	*OutWeaponObject = NewObject<UCWeaponObject>();
	
	ACAttachment* Attachment = nullptr;
	if(!!AttachmentClass)
	{
		FActorSpawnParameters Parameters;
		Parameters.Owner = InOwner;

		Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, Parameters);
	}// if(!!AttachmentClass)

	UCEquipment* Equipment = nullptr;
	if(!!EquipmentClass)
	{
		Equipment = NewObject<UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay(InOwner, EquipmentData);

		if(!!Attachment)
		{
			//Attachment가 있다면 Equip관련 이벤트에 Attachmnet의 함수를 바인딩
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentEndEquip.AddDynamic(Attachment, &ACAttachment::OnEndEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}// if(!!EquipmentClass)

	UCDoAction* DoAction = nullptr;
	if(!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(InOwner, *OutWeaponObject);

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if (!!Equipment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(DoAction, &UCDoAction::OnBeginEquip);
			Equipment->OnEquipmentEndEquip.AddDynamic(DoAction, &UCDoAction::OnEndEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(DoAction, &UCDoAction::OnUnEquip);
		}
	}// if(!!DoActionClass)

	UCDoSubAction* DoSubAction = nullptr;
	if(!!DoSubActionClass)
	{
		DoSubAction = NewObject<UCDoSubAction>(this, DoSubActionClass);
		DoSubAction->BeginPlay(InOwner, *OutWeaponObject, Attachment, DoAction);
	}// if(!!DoSubAction)

	UCEvadeAction* EvadeAction = nullptr;
	if(!!EvadeActionClass)
	{
		EvadeAction = NewObject<UCEvadeAction>(this, EvadeActionClass);
		EvadeAction->BeginPlay(InOwner, EvadeData);

		if(!!Equipment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(EvadeAction, &UCEvadeAction::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(EvadeAction, &UCEvadeAction::OnUnequip);
		}
	}// if(!!EvadeActionClass)

	UCCtrlAction* CtrlAction = nullptr;
	if (!!CtrlActionClass)
	{
		CtrlAction = NewObject<UCCtrlAction>(this, CtrlActionClass);
		CtrlAction->BeginPlay(InOwner, *OutWeaponObject, Attachment);

		if (!!Equipment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(CtrlAction, &UCCtrlAction::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(CtrlAction, &UCCtrlAction::OnUnequip);
		}
	}// if(!!CtrlActionClass)

	(*OutWeaponObject)->Attachment = Attachment;
	(*OutWeaponObject)->Equipment = Equipment;
	(*OutWeaponObject)->DoAction = DoAction;
	(*OutWeaponObject)->DoSubAction = DoSubAction;
	(*OutWeaponObject)->EvadeAction = EvadeAction;
	(*OutWeaponObject)->CtrlAction = CtrlAction;
}