#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"
/**
 * @brief Weapon Data를 관리하는 DataAsset
 */
UCLASS()
class CPROJECT_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);
	
private:
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCEquipment* Equipment;

// Getter
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
};
