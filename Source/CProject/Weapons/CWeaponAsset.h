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

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

//Datas
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FEquipmentData EquipmentData;
	
	UPROPERTY(EditAnywhere, Category = "DoAction")
	TArray<FDoActionData> DoActionDatas;
	
private:
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCEquipment* Equipment;
	UPROPERTY()
	class UCDoAction* DoAction;

//Getter
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
};
