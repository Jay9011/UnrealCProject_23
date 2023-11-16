#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "CEnemy.generated.h"

/**
 *	@brief Enemy Base
 */
UCLASS()
class CPROJECT_API ACEnemy : public ACBaseCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void Hitted();
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

private:
	struct FDamagedData
	{
		float Damage;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} Damaged;
};
