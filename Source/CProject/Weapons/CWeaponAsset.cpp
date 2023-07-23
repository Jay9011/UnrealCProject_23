#include "Weapons/CWeaponAsset.h"

#include "CAttachment.h"
#include "CEquipment.h"
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
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnequip.AddDynamic(Attachment, &ACAttachment::OnUnequip);
		}
	}// if(!!EquipmentClass)
}
