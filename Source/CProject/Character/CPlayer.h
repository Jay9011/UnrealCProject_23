#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Components/CStateComponent.h"
#include "Components/IStateNotify.h"
#include "DebugHeader.h"
#include "CPlayer.generated.h"

/**
 * @brief 플레이어
 */
UCLASS()
class CPROJECT_API ACPlayer : public ACBaseCharacter
	, public IIStateNotify
	, public IIDebugCollector
{
	GENERATED_BODY()

public:
	ACPlayer();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
	
public:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	
// IIStateNotify
public:
	virtual void End_Evade() override;
	virtual void End_Hitted() override;
	virtual void End_Blocking() override;
	virtual void End_Unprotected() override;
	virtual void End_Dead() override;
	
/*
 * 카메라 관련
 */
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

/*
 *	기본 컴포넌트 
 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Base")
	class UCMovementComponent* MovementComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Base")
	class UCWeaponComponent* Weapon;

/*
 * 확장 컴포넌트
 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Extension")
	class UCEvadeComponent* Evade;

	UPROPERTY(VisibleAnywhere, Category = "Extension")
	class UCAirComponent* Air;
	
	UPROPERTY(VisibleAnywhere, Category = "Extension")
	class UHitMontageComponent* HitMontage;

	UPROPERTY(VisibleAnywhere, Category = "Extension")
	class UCGuardComponent* GuardComponent;
	
private:
	float MinimumLiftZ = 100.f;

/*
 * Getter
 */
public:
	USpringArmComponent* GetSpringArm() const { return SpringArm; }
	
#if WITH_EDITOR
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Debug")
	class UDebuggerComponent* Debugger;
#endif
	
#if DEBUG_DEFAULT_INFO
public:
	virtual bool IsDebugEnable() override { return true; } // 플레이어의 디버거 정보는 항상 출력
	virtual FDebugInfo GetDebugInfo() override;
#endif

};
