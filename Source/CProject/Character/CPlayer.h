#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class CPROJECT_API ACPlayer : public ACBaseCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

	UPROPERTY(VisibleDefaultsOnly, Category = "State")
	class UCStateComponent* State;
	
public:
	ACPlayer();
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/*
 * Getters
 */
public:
	const UCStateComponent& GetState() const { return *State; }
};
