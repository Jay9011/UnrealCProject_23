#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
private:
	// Camera
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45;

	// Speed
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::Max] = { 240, 600, 800, 140, 240 };

public:
	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE float GetWalkSpeed() const { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() const { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() const { return Speed[(int32)ESpeedType::Sprint]; }
	FORCEINLINE float GetCrouchWalkSpeed() const { return Speed[(int32)ESpeedType::CrouchWalk]; }
	FORCEINLINE float GetCrouchRunSpeed() const { return Speed[(int32)ESpeedType::CrouchRun]; }

	FORCEINLINE bool GetFixedCamera() const { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }
	
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

private:
	class ACharacter* OwnerCharacter;

private:
	bool bCanMove = true;
	bool bFixedCamera;
};
