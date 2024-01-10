#pragma once

#include "CoreMinimal.h"
#include "Weapons/CCtrlAction.h"
#include "Interface/CDamagedInterface.h"
#include "CGuardAction.generated.h"

class UAnimInstance;
class UCStateComponent;
class UCGuardComponent;
class UCMovementComponent;
class UCGuardMontageAsset;

/**
 * @brief 가드 액션
 */
UCLASS(Abstract, NotBlueprintable)
class CPROJECT_API UCGuardAction : public UCCtrlAction
{
	GENERATED_BODY()
public:
	UCGuardAction();

public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment) override;

	virtual void Pressed() override;
	virtual void Released() override;
	
public:
	virtual void OnBeginEquip() override;
	virtual void OnUnequip() override;
	
/* Blocking */
public:
	void SetBlockingMode();
	void EndBlockingMode();
	UFUNCTION()
	virtual void OnEvaluateBlocking(FDamagedData& DamagedData);
	
	virtual void BlockingSuccess(FDamagedData& DamagedData);
	virtual void GuardBreak(FDamagedData& DamagedData);
		
/* Parrying */
public:
	void SetParryingMode();
	// parrying의 정상 종료
	void EndParryingMode();
	UFUNCTION()
	virtual void OnEvaluateParrying(FDamagedData& DamagedData);
	
	virtual void ParryingSuccess(FDamagedData& DamagedData);

protected:
	// parrying 타이머 제거
	void ClearParryingTimer();
	
/* Check */
public:
	bool CanBlocking() const;

/* Delegate */
public:
	UFUNCTION()
	void OnPlayMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevStateType, EStateType InNewStateType);

protected:
	void MovementProcess(bool CanMove = true, bool FixedCamera = false);
	
	
protected:
	UPROPERTY()
	UCStateComponent* StateComponent;
	UPROPERTY()
	UCMovementComponent* MovementComponent;
	UPROPERTY()
	UCGuardComponent* GuardComponent;
	UPROPERTY()
	UAnimInstance* AnimInstance;
	
	UPROPERTY(EditAnywhere, Category = "Guard")
	UCGuardMontageAsset* GuardMontageAsset;

private:
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingDelay = 0.1f;
	UPROPERTY(EditAnywhere, Category = "Parrying")
	float ParryingDuration = 1.f;

protected:
	bool BlockCancel = false;
	bool ReserveBlockCancel = false;

private:
	FTimerHandle ParryingTimerHandle;
	FTimerHandle ParryingEndTimerHandle;
};
