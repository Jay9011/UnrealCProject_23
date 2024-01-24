#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CAirComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoSubAction.h"
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

/* 기본 애니메이션 정보 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Speed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Pitch = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float Direction = 0.0f;

/* 상태 관리 정보 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EStateType StateType = EStateType::Max;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EStandingType StandingType = EStandingType::Standing;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::Max;

/* Air 관련 정보 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air")
	EAirState AirState = EAirState::Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Air")
	bool isAir = false;

/* IK 관련 정보 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inverse Kinematics")
	bool UseFeetIK = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inverse Kinematics")
	FFeetData FeetData;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head Rotation")
	bool UseNeckRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Head Rotation")
	FRotator NeckRotation;

/* Weapon 관련 정보 */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bSubActionPressed = false;
	
private:
	ACharacter* OwnerCharacter;
	UCStateComponent* State;
	UCWeaponComponent* Weapon;
	UCAirComponent* Air;
	UCMovementComponent* Movement;
	UCFeetComponent* Feet;
	class UCNeckComponent* Neck;

private:
	FRotator PrevRotation;

public:
	void EnableNeckRotation();
	void DisableNeckRotation();
};
