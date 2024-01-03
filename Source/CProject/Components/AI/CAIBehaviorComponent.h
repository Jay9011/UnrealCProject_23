#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM()
enum class EAIStateType : uint8
{
	Wait = 0,
	Patrol,
	Approach,
	Action, Hitted,
	Evade,
	Dead,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS()
class CPROJECT_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

	/*
	 * Delegate
	 */
public:
	FAIStateChanged OnAIStateChanged;
	
private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";
	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";

private:
	class UBlackboardComponent* Blackboard = nullptr;

	/*
	 * Blackboard Getter / Setter
	 */
public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
	
	FORCEINLINE FName GetAIStateTypeKey() const { return AIStateTypeKey; }
	FORCEINLINE FName GetTargetKey() const { return TargetKey; }

public:
	EAIStateType GetAIStateType() const;
	void ChangeAIState(EAIStateType InType);
	
	ACharacter* GetTarget();
	
	FVector GetPatrolLocation() const;
	void SetPatrolLocation(const FVector& InLocation);

	/*
	 * State Getter / Setter
	 */
public:
	bool IsWait() const;
	bool IsPatrol() const;
	bool IsApproach() const;
	bool IsAction() const;
	bool IsHitted() const;
	bool IsEvade() const;
	bool IsDead() const;

	void SetWaitMode();
	void SetPatrolMode();
	void SetApproachMode();
	void SetActionMode();
	void SetHittedMode();
	void SetEvadeMode();
	void SetDeadMode();
};