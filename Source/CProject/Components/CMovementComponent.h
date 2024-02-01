#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "Character/DataAssets/CStandUpAsset.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, CrouchWalk, CrouchRun, Max
};

UENUM(BlueprintType)
enum class EStandingType : uint8
{
	Standing = 0, Crouch, KnockDown, Max
};


// Delegate List
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStandingTypeChanged, EStandingType, InPrevType, EStandingType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCMovementComponent : public UActorComponent
	, public IIDebugCollector
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

public:
	UFUNCTION(BlueprintCallable)
	void EnableControlRotation();
	UFUNCTION(BlueprintCallable)
	void DisableControlRotation();

public:
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
	 * 서있는 상태 관련
	 */
public:
	virtual void SetStandingMode();
	virtual void SetCrouchMode();
	virtual void SetKnockDownMode();

	void SwitchCrouchMode();
	
	void OnCrouch();
	void OffCrouch();

	void StandUp(EDir InDir);

	/*
	 * Delegate 실행 함수
	 */
protected:
	virtual void ChangeStandingType(EStandingType InType);
	
/*
 * Getter / Setter
 */
public:
	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE virtual void Move();
	FORCEINLINE virtual void Stop();

	FORCEINLINE bool GetFixedCamera() const { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }
	
	FORCEINLINE float GetWalkSpeed() const { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() const { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() const { return Speed[(int32)ESpeedType::Sprint]; }
	FORCEINLINE float GetCrouchWalkSpeed() const { return Speed[(int32)ESpeedType::CrouchWalk]; }
	FORCEINLINE float GetCrouchRunSpeed() const { return Speed[(int32)ESpeedType::CrouchRun]; }

	
	/*
	 * Standing Type 관련
	 */
	FORCEINLINE EStandingType GetStandingType() const { return StandingType; }

	FORCEINLINE bool IsStand() const { return StandingType == EStandingType::Standing; }
	FORCEINLINE bool IsCrouch() const { return StandingType == EStandingType::Crouch; }
	FORCEINLINE bool IsKnockDown() const { return StandingType == EStandingType::KnockDown; }

	FORCEINLINE bool IsDown() const { return StandingType == EStandingType::KnockDown; }

	FORCEINLINE bool bStandingProcessIsDone() const { return !bStandingProcess; }
	
/*
 * Delegate
 */
public:
	FStandingTypeChanged OnStandingTypeChanged;
	
private:
	// Camera
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool ControlRotation = false;
	
	UPROPERTY(EditAnywhere, Category = "Camera|CameraSpeed")
	float HorizontalLook = 45;

	UPROPERTY(EditAnywhere, Category = "Camera|CameraSpeed")
	float VerticalLook = 45;

	// Speed
	UPROPERTY(EditAnywhere, Category = "Speed")
	ESpeedType InitSpeedType = ESpeedType::Run;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::Max] = { 240, 600, 800, 140, 240 };

	// Standing
	UPROPERTY(EditAnywhere, Category = "Standing")
	UCStandUpAsset* StandUpAsset = nullptr;
	
private:
	bool bCanMove = true;
	bool bFixedCamera = false;

private:
	bool bBackupControlRotation = false;
	bool bBackupRotationYaw = false;
	bool bBackupOrientRotationToMovement = false;
	
protected:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

	EStandingType  StandingType;
	bool bStandingProcess = false;

#if DEBUG_MOVEMENT
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
	
};
