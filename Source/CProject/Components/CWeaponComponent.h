#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "CWeaponComponent.generated.h"

/**
 * @brief 무기 타입
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Blade,
	Max UMETA(DisplayName = "Unarmed") // UEnum으로 문자열로 표시될 때 Unarmed로 표시되도록 함
};

/*
 * Delegate
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

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
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	// 액션 동작
	void DoAction();
	// 보조 액션 동작 중 누르고 있는 동작
	void SubAction_Pressed();
	// 보조 액션 동작 중 버튼을 뗄 때의 동작
	void SubAction_Released();
	
public:
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	class UCDoAction* GetDoAction();
	class UCDoSubAction* GetSubAction();
	class IIExcuteAction* GetCurrentAction();
	class IIExcuteAction* GetReservedAction();
	
public:
	void SetUnarmedMode();
	void SetBladeMode();
	
private:
	bool IsIdleMode();

/*
 * Member
 */
private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* WeaponAssets[static_cast<int32>(EWeaponType::Max)];
	
private:
	class ACharacter* OwnerCharacter;
	EWeaponType Type = EWeaponType::Max;

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

/*
 * Getter
 */
public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsBladeMode() { return Type == EWeaponType::Blade; }
	
	FORCEINLINE EWeaponType GetType() { return Type; }

/*
 * Debugger
 */
#if WITH_EDITOR
public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
