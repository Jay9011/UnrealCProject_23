#pragma once

#include "CoreMinimal.h"
#include "Components/IEvadeAction.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"
/**
 * @brief Weapon Data를 관리하는 DataAsset
 */
UCLASS()
class CPROJECT_API UCWeaponAsset : public UDataAsset
{
private:
	GENERATED_BODY()

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);
	
private:
	//사용 할 클래스 선택
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEvadeAction> EvadeActionClass;
	
private:
	//Datas
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FEquipmentData EquipmentData;
	
	UPROPERTY(EditAnywhere, Category = "DoAction")
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Evade")
	TArray<FEvadeData> EvadeData;
	
private:
	//실제 사용할 인스턴스
	UPROPERTY()
	class ACAttachment* Attachment;
	UPROPERTY()
	class UCEquipment* Equipment;
	UPROPERTY()
	class UCDoAction* DoAction;
	UPROPERTY()
	class UCEvadeAction* EvadeAction;

//Getter
public:
	FORCEINLINE class ACAttachment* GetAttachment() const { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCEvadeAction* GetEvadeAction() { return EvadeAction; }
};
