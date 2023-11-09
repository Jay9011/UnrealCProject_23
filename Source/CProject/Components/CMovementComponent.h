#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk, Run, Sprint, CrouchWalk, CrouchRun, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetSpeed(ESpeedType InType);

	void OnWalk();
	void OnRun();
	void OnSprint();
	void OnCrouchWalk();
	void OnCrouchRun();

	void EnableControlRotation();
	void DisableControlRotation();

	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

public:
	// ControlRotation을 저장.
	void BackupControlRotation();
	// 저장된 ControlRotation을 복구. (이미 저장된 값을 사용하기 때문에, BackupControlRotation을 먼저 호출해야함.) 
	void RestoreControlRotation();
	
/*
 * Getter / Setter
 */
public:
	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE bool GetFixedCamera() const { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }
	
	FORCEINLINE float GetWalkSpeed() const { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() const { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() const { return Speed[(int32)ESpeedType::Sprint]; }
	FORCEINLINE float GetCrouchWalkSpeed() const { return Speed[(int32)ESpeedType::CrouchWalk]; }
	FORCEINLINE float GetCrouchRunSpeed() const { return Speed[(int32)ESpeedType::CrouchRun]; }

private:
	// Camera
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45;

	// Speed
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::Max] = { 240, 600, 800, 140, 240 };

private:
	bool bCanMove = true;
	bool bFixedCamera = false;

private:
	bool bBackupRotationYaw = false;
	bool bBackupOrientRotationToMovement = false;
	
private:
	class ACharacter* OwnerCharacter;
};
