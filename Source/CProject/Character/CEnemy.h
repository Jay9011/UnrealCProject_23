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

protected:
	virtual void ApplyDamage() override;
	virtual void LaunchEffect(const FRotator& InLookAtRotation) override;
	virtual void LaunchAttacker(const FRotator& InLookAtRotation) override;
	virtual void ResetDamagedData(FDamagedData& DamagedData) override;
	virtual void Hitted() override;
	virtual void Dead() override;
	
private:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

	// IIStateNotify
public:
	virtual void End_Hitted() override;
	virtual void End_Blocking() override;
	virtual void End_Unprotected() override;
	virtual void End_Dead() override;
	
protected:
	UPROPERTY()
	class UCAirComponent* Air;
	
	UPROPERTY()
	class UHitMontageComponent* HitMontage;
	
	UPROPERTY()
	class UCMovementComponent* MovementComponent;

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
