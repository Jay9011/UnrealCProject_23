#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeaponStructures.h"
#include "UObject/NoExportTypes.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

/**
 * @brief 장착 관련 클래스
 */
UCLASS()
class CPROJECT_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Equipment")
	void Unequip();
	void Unequip_Implementation();

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip OnEquipmentUnequip;
	
public:
	class ACharacter* OwnerCharacter;
	FEquipmentData Data;

private:
	class UCMovementComponent* MovementComponent;
	class UCStateComponent* StateComponent;
	
private:
	bool bBeginEquip;
	bool bEquipped;

	/*
	 * Getter
	 */
public:
	FORCEINLINE bool IsEquipped() const { return bEquipped; }
	FORCEINLINE bool IsBeginEquip() const { return bBeginEquip; }
};
