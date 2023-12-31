#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "CWeaponComponent.generated.h"

class UCWeaponAsset;

/**
 * @brief 무기 타입
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Blade,
	Max UMETA(DisplayName = "Unarmed") // UEnum으로 문자열로 표시될 때 Unarmed로 표시되도록 함
};

/**
 * @brief 장비 타입
 */
UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	MainWeapon UMETA(DisplayName = "Main Weapon"),
	SubWeapon UMETA(DisplayName = "Sub Weapon"),
	Max UMETA(DisplayName = "UnEquipped")
};

/*
 * Delegate
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipSlotTypeChanged, EEquipSlotType, InPrevType, EEquipSlotType, InNewType);

/**
 * @brief 무기 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCWeaponComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void SetMode(EEquipSlotType InSlotType);
	void ChangeType(EEquipSlotType InType);

public:
	// 액션 동작
	UFUNCTION(BlueprintCallable, Category = "Action")
	void DoAction();
	// 보조 액션 동작 중 누르고 있는 동작
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SubAction_Pressed();
	// 보조 액션 동작 중 버튼을 뗄 때의 동작
	UFUNCTION(BlueprintCallable, Category = "Action")
	void SubAction_Released();
	// Ctrl 키를 누르고 있는 동안의 동작
	UFUNCTION(BlueprintCallable, Category = "Control")
	void Ctrl_Pressed();
	// Ctrl 키를 뗄 때의 동작
	UFUNCTION(BlueprintCallable, Category = "Control")
	void Ctrl_Released();
	
public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCDoSubAction* GetSubAction();
	class UCCtrlAction* GetCtrlAction();
	
	class IIExcuteAction* GetCurrentAction();
	class IIExcuteAction* GetReservedAction();
	
public:
	void SetUnarmedMode();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetMainWeaponMode();
	
private:
	bool IsIdleMode();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	FEquipSlotTypeChanged OnEquipSlotTypeChanged;

/*
 * Member
 */
private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* WeaponAssets[static_cast<int32>(EEquipSlotType::Max)] = {nullptr,};

	UPROPERTY()
	class UCWeaponObject* WeaponObject[static_cast<int32>(EEquipSlotType::Max)] = {nullptr,};
	
private:
	UPROPERTY()
	class ACharacter* OwnerCharacter = nullptr;

	EEquipSlotType Type = EEquipSlotType::Max;

/*
 * Getter
 */
	// 장착 부위 타입
public:
	FORCEINLINE bool IsUnarmedMode() const { return Type == EEquipSlotType::Max; }
	FORCEINLINE bool IsMainWeaponMode() const { return Type == EEquipSlotType::MainWeapon; }
	FORCEINLINE bool IsSubWeaponMode() const { return Type == EEquipSlotType::SubWeapon; }

	FORCEINLINE EEquipSlotType GetType() const { return Type; }
	
	// 무기 타입
public:
	FORCEINLINE bool IsBladeMode() const;

	FORCEINLINE EWeaponType GetWeaponType() const;

	/*
 * Debugger
 */
#if DEBUG_WEAPON_COMPONENT
public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
