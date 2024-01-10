#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

class ACBaseCharacter;
class UAISenseConfig_Sight;
class UAIPerceptionComponent;
class UCAIBehaviorComponent;
class ACEnemy_AI;

/**
 * @brief AI Controller Base
 */
UCLASS()
class CPROJECT_API ACAIController : public AAIController
{
	GENERATED_BODY()
public:
	ACAIController();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void SetFacialFocus(ACBaseCharacter* InActor);
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
	bool CheckNeckComponent(ACharacter* Actor);
	void CheckNeckThreshold(const ACharacter* Actor) const;

	/*
	 * Delegate
	 */
private:
	// 일반 감지시
	UFUNCTION()
	void OnPerceptionUpdatedDelegate(const TArray<AActor*>& UpdatedActors);
	
private:
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* Perception = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float SightRadius = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float LoseSightRadius = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float PeripheralVisionAngleDegrees = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float SightAge = 5.f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Neck")
	float NeckThreshold = 0.1f;
	
private:
	UPROPERTY()
	UAISenseConfig_Sight* Sight = nullptr;
	
	TWeakObjectPtr<ACEnemy_AI> Enemy = nullptr;
	TWeakObjectPtr<UCAIBehaviorComponent> Behavior = nullptr;

	bool bFocusFacial = false;
	TWeakObjectPtr<ACBaseCharacter> FocusFacialActor = nullptr;

	/*
	 * Getter
	 */
public:
	FORCEINLINE UCAIBehaviorComponent* GetBehaviorComponent() const;
};
