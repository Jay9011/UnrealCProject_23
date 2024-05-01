#pragma once

#include "CoreMinimal.h"
#include "Components/HitMontageComponent.h"
#include "Particles/ParticleSystem.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

/*
 * @brief Gravity 관련 데이터 셋
 */
USTRUCT()
struct FGravityData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float GravityScale = 1;

	UPROPERTY(EditAnywhere)
	FVector SelfVelocity = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float RecoveryTime = 0;
};

/**
 * @brief DamageEvent와 관련된 데이터 셋
 */
USTRUCT()
struct FHitData : public FTableRowBase
{
	GENERATED_BODY()
	// 기본 데이터
public:
	UPROPERTY(EditAnywhere, Category = "Damage")
	bool bGuardBreakable = false;
	
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 0;

	UPROPERTY(EditAnywhere, Category = "Damage")
	EHitType HitType = EHitType::Normal;

	UPROPERTY(EditAnywhere, Category = "Damage")
	bool isAirborne = false;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FVector DamagedLaunch = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Damage")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, Category = "Damage")
	bool bSuspensionInAir = false;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FGravityData GravityData;
	
	UPROPERTY(EditAnywhere, Category = "Attaker")
	FVector AttackerLaunch = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Attaker")
	bool bAttackerSuspensionAir = true;

	UPROPERTY(EditAnywhere, Category = "Attaker")
	FGravityData AttackerGravityData;
	
	// 피격시 피격자의 행동 정보
	UPROPERTY(EditAnywhere, Category = "Animation")
	float PlayRate = 1;

	// 피격시 특수 효과
	UPROPERTY(EditAnywhere, Category = "Time")
	float HitStopTime = 0;
	
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* Sound;

	UPROPERTY(EditAnywhere, Category = "Camera")
	TSubclassOf<UMatineeCameraShake> Shake;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayShake(AController* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);
};

/**
 * @brief 장착 관련 데이터 셋
 */
USTRUCT()
struct FEquipmentData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;
};

/**
 * @brief DoAction과 관련된 데이터 셋
 */
USTRUCT(BlueprintType)
struct FDoActionData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float PlayRate = 1;

	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector MovementLaunch = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bCanMove = true;

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bFixedCamera = false;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectScale = FVector::OneVector;

	// 데미지 관련 데이터 셋
	UPROPERTY(EditAnywhere, Category = "Damage")
	FHitData DamagedData;

public:
	void DoAction(class ACharacter* InOwner);
};

/*
 * @brief DamageEvent 전달용
 */
USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()
public:
	FHitData* HitData;
	UPrimitiveComponent* OverlappedComponent;
};

/*
 * @brief Action 데이터 관리
 */
USTRUCT()
struct FActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* ActionDataTable = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<FDoActionData> ActionData;

	// 현재 데미지를 입은 캐릭터들
	TArray<class ACharacter*> DamagedCharacters;

public:
	void InitActionData();
};

UCLASS()
class CPROJECT_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
