#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
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
	void SightTargetInEvent(AActor* Actor, FAIStimulus& Stimulus);
	// 타겟을 시야에서 잃어버린 경우
	void SightTargetLostEvent(AActor* Actor, FAIStimulus& Stimulus);
	// 타겟을 완전히 잃는 경우
	void SightTargetForgetEvent(FAIStimulus& Stimulus);
	// 새로운 듣기가 감지된 경우
	void HearingEvent(AActor* Actor, FAIStimulus& Stimulus);
	// 경계 중 듣기가 감지된 경우
	void VigilanceHearingEvent(AActor* Actor, FAIStimulus& Stimulus);
	
	/*
	 * Delegate
	 */
private:
	// 감지된 타겟의 변경점이 있을 때(Actor와 FAIStimulus 사용)
	UFUNCTION()
	void OnTargetPerceptionUpdatedDelegate(AActor* Actor, FAIStimulus Stimulus);
	// 감지 만료시
	UFUNCTION()
	void OnHandleExpiredDelegate(FAIStimulus& StimulusStore);
	
private:
	UPROPERTY(VisibleAnywhere)
	UCAIPerceptionComponentAddExpired* Perception = nullptr;

	/* Hearing */
	UPROPERTY(EditDefaultsOnly, Category = "Perception|Hearing")
	FName HearingBlackboardKey = "Hearing";
	
protected:
	UPROPERTY()
	UAISenseConfig_Sight* Sight = nullptr;
	UPROPERTY()
	UAISenseConfig_Hearing* Hearing = nullptr;

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
