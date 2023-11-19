#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, CrouchWalk, CrouchRun, Max
};

UENUM()
enum class EStandingType : uint8
{
	Standing = 0, Crouch, KnockDown, Max
};

UENUM()
enum class EAirState : uint8
{
	Normal = 0, Jump, Fall, Airborne, MAX
};

// Delegate List
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAirStateChanged, EAirState, InPrevState, EAirState, InNewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStandingTypeChanged, EStandingType, InPrevType, EStandingType, InNewType);

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
	 * 공중 상태 관련
	 */
public:
	// 공중 상태가 아닌 일반 상태
	void SetLandMode();
	void SetJumpMode();
	void SetFallMode();
	// 적에게 맞아서 날아가는 상태
	void SetAirborneMode();

	/*
	 * 서있는 상태 관련
	 */
public:
	void SetStandingMode();
	void SetCrouchMode();
	void SetKnockDownMode();

	/*
	 * Delegate 실행 함수
	 */
private:
	void ChangeAirState(EAirState InState);
	void ChangeStandingType(EStandingType InType);
	
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

	/*
	 * 공중 상태 관련
	 */
	FORCEINLINE EAirState GetAirState() const { return AirState; }
	
	FORCEINLINE bool IsAir() const { return AirState != EAirState::Normal; }
	FORCEINLINE bool IsJump() const { return AirState == EAirState::Jump; }
	FORCEINLINE bool IsFall() const { return AirState == EAirState::Fall; }
	FORCEINLINE bool IsAirborne() const { return AirState == EAirState::Airborne; }
	FORCEINLINE bool IsAerialStance() const { return AirState == EAirState::Jump || AirState == EAirState::Fall; }

	/*
	 * Standing Type 관련
	 */
	FORCEINLINE EStandingType GetStandingType() const { return StandingType; }

	FORCEINLINE bool IsStand() const { return StandingType == EStandingType::Standing; }
	FORCEINLINE bool IsCrouch() const { return StandingType == EStandingType::Crouch; }
	FORCEINLINE bool IsKnockDown() const { return StandingType == EStandingType::KnockDown; }
	
/*
 * Delegate
 */
public:
	FAirStateChanged OnAirStateChanged;
	FStandingTypeChanged OnStandingTypeChanged;
	
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

	EAirState AirState;
	EStandingType StandingType;
};
