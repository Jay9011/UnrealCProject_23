#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "Components/IEvadeAction.h"
#include "CWeaponAsset.generated.h"

/**
 * @brief Weapon Data를 관리하는 DataAsset
 * @details Data를 공유하는 용도로 사용
 */
UCLASS()
class CPROJECT_API UCWeaponAsset : public UDataAsset
{
private:
	GENERATED_BODY()

public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner, class UCWeaponObject** OutWeaponObject);

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Max;
	
private:
	/*
	 * 사용 할 클래스 선택용
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoSubAction> DoSubActionClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEvadeAction> EvadeActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCCtrlAction> CtrlActionClass;
	
private:
	//Datas
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FEquipmentData EquipmentData;
	
	UPROPERTY(EditAnywhere, Category = "Evade")
	TArray<FEvadeData> EvadeData;

/*
 * Getter / Setter
 */
public:
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
