#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Components/CStateComponent.h"
#include "Components/IStateNotify.h"
#include "DebugHeader.h"
#include "GenericTeamAgentInterface.h"
#include "Components/CWeaponComponent.h"
#include "CPlayer.generated.h"

/**
 * @brief 플레이어
 */
UCLASS()
class CPROJECT_API ACPlayer : public ACBaseCharacter
	, public IIStateNotify
	, public IGenericTeamAgentInterface
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

// IGenericTeamAgentInterface
public:
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

// Weapon Change 관련
public:
	UFUNCTION()
	void WeaponChange_MainWeapon();
	UFUNCTION()
	void WeaponChange_SubWeapon();
	UFUNCTION()
	void WeaponChange_ThirdWeapon();
	
	void WeaponChangeAction(EEquipSlotType InSlotType);
	
/*
 * 카메라 관련
 */
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UChildActorComponent* MainCam;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* SceneCamera;

	UPROPERTY(VisibleAnywhere)
	class UChildActorComponent* SceneCam;

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

	UPROPERTY(VisibleAnywhere, Category = "Extension")
	class UCInteractiveComponent* InteractiveComponent;
	
private:
	float MinimumLiftZ = 100.f;

/*
 * Getter
 */
public:
	USpringArmComponent* GetSpringArm() const { return SpringArm; }
	UChildActorComponent* GetMainCam() const { return MainCam; }
	UChildActorComponent* GetSceneCam() const { return SceneCam; }
	
#if WITH_EDITOR
public:
	UPROPERTY()
	class UDebuggerComponent* Debugger;
#endif
	
#if DEBUG_DEFAULT_INFO
public:
	virtual bool IsDebugEnable() override { return true; } // 플레이어의 디버거 정보는 항상 출력
	virtual FDebugInfo GetDebugInfo() override;
#endif

};
