#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

/**
 * @brief DamageEvent와 관련된 데이터 셋
 */
USTRUCT()
struct FHitData
{
	GENERATED_BODY()
	// 기본 데이터
	public:
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 0;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FVector DamagedLaunch = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Damage")
	bool bCanMove = true;
	
	// 피격시 피격자의 행동 정보
	public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float PlayRate = 1;

	// 피격시 특수 효과
	UPROPERTY(EditAnywhere, Category = "Effect")
	float StopTime = 0;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	class USoundWave* Sound;
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Effect")
	FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);
	void PlayMontage(class ACharacter* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACharacter* InOwner);
};

/**
 * @brief 장착 관련 데이터 셋
 */
USTRUCT()
struct FEquipmentData
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
USTRUCT()
struct FDoActionData
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
};

UCLASS()
class CPROJECT_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
