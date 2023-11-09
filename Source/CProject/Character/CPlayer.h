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

/*
 * State 관련
 */
private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

// IIStateNotify
public:
	virtual void End_Evade() override;
	
#if WITH_EDITOR
public:
	virtual bool IsDebugEnable() override { return true; } // 플레이어의 디버거 정보는 항상 출력
	virtual FDebugInfo GetDebugInfo() override;
#endif

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere, Category = "Evade")
	class UCEvadeComponent* Evade;

#if WITH_EDITOR
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Debug")
	class UDebuggerComponent* Debugger;
#endif
};
