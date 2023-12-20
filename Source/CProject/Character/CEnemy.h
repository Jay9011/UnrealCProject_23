#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "DebugHeader.h"
#include "Components/IStateNotify.h"
#include "CEnemy.generated.h"

/**
 *	@brief Enemy Base
 */
UCLASS()
class CPROJECT_API ACEnemy : public ACBaseCharacter
	, public IIStateNotify
	, public IIDebugCollector
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Falling() override;
	
public:
	virtual void PlayHittedMontage() override;
	void AirSuspension(ACharacter& Character, ACharacter& Attacker);
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void Hitted();
	virtual void End_Hitted() override;
	void Dead();
	virtual void End_Dead() override;

private:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

private:
	struct FDamagedData
	{
		float Damage;
		class ACharacter* Character;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} Damaged;

private:
	UPROPERTY()
	class UCAirComponent* Air;
	class UHitMontageComponent* HitMontage;
	UPROPERTY()
	class UCMovementComponent* MovementComponent;
	UPROPERTY()
	class UCStatusComponent* StatusComponent;

private:
	float MinimumLiftZ = 100.f;

/*
 * IDebugCollector
 */
#if WITH_EDITOR
private:
	UPROPERTY()
	class UDebuggerComponent* Debugger;
#endif

#if DEBUG_DEFAULT_INFO
public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
