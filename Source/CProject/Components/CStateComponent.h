#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	Equip, Action, Hitted, Blocking, Parrying, Parryed,
	Evade,
	Parkour,
	Dead, Wait,
	Max
};

// Delegate List
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

/*
 * State Component
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCStateComponent : public UActorComponent
	, public IIDebugCollector
{
private:
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType(EStateType InType);

public:
	void SetIdleMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetEvadeMode();
	
	/*
	 * 서브 액션은 다른 동작 중에도 할 수 있게 하기 위해 따로 처리
	 */
	void OnSubActionMode() { bInSubActionMode = true; };
	void OffSubActionMode() { bInSubActionMode = false; };

public:
	FORCEINLINE bool IsIdleMode() const { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEquipMode() const { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() const { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() const { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() const { return Type == EStateType::Action; }
	FORCEINLINE bool IsEvadeMode() const { return Type == EStateType::Evade; }

	FORCEINLINE bool IsSubActionMode() const { return bInSubActionMode; }

/*
 * Getters
 */
public:
	FORCEINLINE EStateType GetType() const { return Type; }
	
public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
	bool bInSubActionMode = false; // subAction을 처리하기 위한 별도의 상태(Enum과 별개로 처리)

#if WITH_EDITOR
public:
	virtual bool IsDebugEnable() override { return true; } // 상태 여부는 항상 디버깅
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
