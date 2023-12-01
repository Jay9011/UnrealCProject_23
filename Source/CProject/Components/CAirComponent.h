#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAirComponent.generated.h"

UENUM(BlueprintType)
enum class EAirState : uint8
{
	Normal = 0, Jump, Fall, Airborne, MAX
};

// Delegate List
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAirStateChanged, EAirState, InPrevState, EAirState, InNewState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCAirComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAirComponent();

protected:
	virtual void BeginPlay() override;

public:
	/*
	 * @brief 중력을 조절하여 일정 시간 동안 공중에 떠있게 한다.
	 * @param InGravityScale 중력 스케일
	 * @param InVelocity 중력 조절 후 캐릭터가 가지게 될 속도
	 * @param InRecoveryTime 중력 조절 후 복구 시간
	 */
	UFUNCTION(BlueprintCallable, Category = "Air")
	void SetGravityScale(float InGravityScale, FVector InVelocity, float InRecoveryTime);
	/*
	 * @brief 중력을 복구한다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Air")
	void RecoveryGravityScale();

public:
	void UpdateOriginalGravityScale();

	void GravityRecoveryTimerCancel(FTimerManager& InTimerManager);
	
/*
 * Delegate
 */
public:
	FAirStateChanged OnAirStateChanged;

/*
 * Delegate 실행 함수
 */
private:
	void ChangeAirState(EAirState InState);
	
/*
* Delegate 전달 함수
*/
private:
	UFUNCTION()
	void OnFLanded(const FHitResult& Hit);
	
private:
	class ACharacter* OwnerCharacter;
	float OriginalGravityScale;

	EAirState AirState;
	FTimerHandle GravityRecoveryTimerHandle;

/*
 * Getter / Setter
 */
public:
	/*
	 * 공중 상태 관련
	 */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EAirState GetAirState() const { return AirState; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAir() const { return AirState != EAirState::Normal; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsJump() const { return AirState == EAirState::Jump; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFall() const { return AirState == EAirState::Fall; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAirborne() const { return AirState == EAirState::Airborne; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAerialStance() const { return AirState == EAirState::Jump || AirState == EAirState::Fall; }

	// 공중 상태가 아닌 일반 상태
	void SetLandMode();
	void SetJumpMode();
	void SetFallMode();
	// 적에게 맞아서 날아가는 상태
	void SetAirborneMode();
};
