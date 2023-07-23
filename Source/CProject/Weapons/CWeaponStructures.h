#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.generated.h"

/**
 * 장착 관련 데이터 셋
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
	UPROPERTY(EditAnywhere)
	class UAnimMontage* ActionMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera = false;
	
	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void DoAction(class ACharacter* InOwner);
};

UCLASS()
class CPROJECT_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
	
};
