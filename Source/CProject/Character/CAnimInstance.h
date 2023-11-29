#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CAirComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class CPROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

	UFUNCTION()
	void OnAirStateChanged(EAirState InPrevState, EAirState InNewState);

	UFUNCTION()
	void OnStandingTypeChanged(EStandingType InPrevType, EStandingType InNewType);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Pitch = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction = 0.0f;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EStateType StateType = EStateType::Max;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EAirState AirState = EAirState::Normal;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool isAir = false;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EStandingType StandingType = EStandingType::Standing;
	
private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class UCWeaponComponent* Weapon;
	class UCAirComponent* Air;
	class UCMovementComponent* Movement;

private:
	FRotator PrevRotation;
};
