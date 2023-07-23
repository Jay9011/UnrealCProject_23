#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

/**
 * @brief 무기 타입
 */
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Blade,
	Max
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
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* WeaponAsset[static_cast<int32>(EWeaponType::Max)];
	
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
	class ACAttachment* GetAttachment();
	class UCEquipment* GetEquipment();
	
public:
	void SetUnarmedMode();
	void SetBladeMode();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	class ACharacter* OwnerCharacter;
	EWeaponType Type = EWeaponType::Max;

/*
 * Getter
 */
private:
	bool IsIdleMode();
	
public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::Max; }
	FORCEINLINE bool IsBladeMode() { return Type == EWeaponType::Blade; }
};
