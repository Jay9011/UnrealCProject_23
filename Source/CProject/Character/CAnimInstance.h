#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class CPROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::Max;
	
private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;

private:
	FRotator PrevRotation;
};
