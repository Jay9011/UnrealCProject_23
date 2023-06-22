#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	Equip, Action, Hitted, Blocking, Parrying, Parryed,
	BackStep, Roll,
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
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	void ChangeType(EStateType InType);

public:
	void SetIdleMode();
	void SetBackStepMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	
public:
	FORCEINLINE bool IsIdleMode() const { return Type == EStateType::Idle; }
	FORCEINLINE bool IsBackstepMode() const { return Type == EStateType::BackStep; }
	FORCEINLINE bool IsEquipMode() const { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() const { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() const { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() const { return Type == EStateType::Action; }

/*
 * Getters
 */
public:
	FORCEINLINE EStateType GetType() const { return Type; }
};
