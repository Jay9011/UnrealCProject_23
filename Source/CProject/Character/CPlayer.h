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
