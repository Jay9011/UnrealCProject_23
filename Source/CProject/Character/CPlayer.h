#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Components/CStateComponent.h"
#include "Components/IStateNotify.h"
#include "CPlayer.generated.h"

UCLASS()
class CPROJECT_API ACPlayer
	: public ACBaseCharacter
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
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;
	
// IIStateNotify
public:
	virtual void End_Evade() override;
	
#if WITH_EDITOR
public:
	virtual bool IsDebugEnable() override { return true; } // 플레이어의 디버거 정보는 항상 출력
	virtual FDebugInfo GetDebugInfo() override;
#endif

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
	class UCMovementComponent* Movement;
	
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

#if WITH_EDITOR
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Debug")
	class UDebuggerComponent* Debugger;
#endif
};
