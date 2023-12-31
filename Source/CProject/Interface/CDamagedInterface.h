#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CDamagedInterface.generated.h"

USTRUCT(BlueprintType)
struct FDamagedData
{
	GENERATED_BODY()
public:
	float Damage;
	class ACharacter* Character;
	class AActor* Causer;

	struct FActionDamageEvent* Event;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCDamagedInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Damaged 관련 인터페이스
 */
class CPROJECT_API ICDamagedInterface
{
	GENERATED_BODY()

public:
	virtual void PlayHittedMontage() {}

protected:
	virtual void Hitted() {}
	virtual void ApplyDamage() {}
	virtual void LaunchEffect(const FRotator& InLookAtRotation) {}
	virtual void LaunchAttacker(const FRotator& InLookAtRotation) {}
	virtual void ResetDamagedData(FDamagedData& DamagedData) {}
	virtual void Dead() {}
};
