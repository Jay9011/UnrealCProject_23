#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "CAIController.generated.h"

class UCAIPerceptionComponentAddExpired;
class ACBaseCharacter;
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

public:
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void SetFacialFocus(ACBaseCharacter* InActor);
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
	bool CheckNeckComponent(ACharacter* Actor);
	void CheckNeckThreshold(const ACharacter* Actor) const;


public:
	/**
	 * @brief 새로운 타겟으로 업데이트 한다.
	 * @return ture : 업데이트 후 타겟이 존재함, false : 업데이트 후 타겟이 존재하지 않음
	 */
	bool UpdateCurrentTarget();

	/*
	 * 감시 센서마다 수행하는 함수
	 */
public:
	// 타겟이 시야에 들어온 경우
	void SightTargetInEvent(AActor* Actor, const FAIStimulus& Stimulus);
	// 타겟을 시야에서 잃어버린 경우
	void SightTargetLostEvent(AActor* Actor, const FAIStimulus& Stimulus);
	// 타겟을 완전히 잃는 경우
	void SightTargetForgetEvent(const FAIStimulus& Stimulus);
	// 새로운 듣기가 감지된 경우
	void NewHearingEvent(AActor* Actor, const FAIStimulus& Stimulus);
	// 경계 중 듣기가 감지된 경우
	void VigilanceHearingEvent(AActor* Actor, const FAIStimulus& Stimulus);
	
	// 타겟을 추적 중(잃어버린 타겟이 있는 경우), 탐색 위치의 변경
	void UpdateLostTargetLocationEvent(const FVector& Location);
	
	
	/*
	 * Delegate
	 */
private:
	// 일반 감지시
	UFUNCTION()
	void OnPerceptionUpdatedDelegate(const TArray<AActor*>& UpdatedActors);
	// 감지된 타겟의 변경점이 있을 때(Actor와 FAIStimulus 사용)
	UFUNCTION()
	void OnTargetPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus);
	// 감지 만료시
	UFUNCTION()
	void OnHandleExpiredDelegate(FAIStimulus& StimulusStore);
	
private:
	UPROPERTY(VisibleAnywhere)
	UCAIPerceptionComponentAddExpired* Perception = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float SightRadius = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float LoseSightRadius = 800.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float PeripheralVisionAngleDegrees = 45.f;
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Sight")
	float SightAge = 5.f;

protected:
	TWeakObjectPtr<UAISenseConfig_Sight> Sight = nullptr;

	bool Vigilance = false;
	
	TWeakObjectPtr<ACEnemy_AI> Enemy = nullptr;
	TWeakObjectPtr<UCAIBehaviorComponent> Behavior = nullptr;

	bool bFocusFacial = false;
	TWeakObjectPtr<ACBaseCharacter> FocusFacialActor = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI|Neck")
	float NeckThreshold = 0.1f;
	
	/*
	 * Getter
	 */
public:
	FORCEINLINE UCAIBehaviorComponent* GetBehaviorComponent() const;
};
