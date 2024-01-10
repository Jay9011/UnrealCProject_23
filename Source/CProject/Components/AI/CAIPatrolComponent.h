#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "CAIPatrolComponent.generated.h"

class UCAnimInstance;
class UCMovementComponent;
class ACEnemy_AI;
class ACPatrolPath;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCAIPatrolComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCAIPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
public:
	UFUNCTION()
	void PlaySearchMontage();

private:
	UFUNCTION()
	void EndSearchMontage(UAnimMontage* Montage, bool bInterrupted);
	
	void AttachedControllerOnHead();
	
private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	ACPatrolPath* PatrolPath = nullptr;

	UPROPERTY(EditAnywhere, Category = "Patrol", meta = (EditCondition = "PatrolPath = nullptr"))
	FTransform PatrolPoint = FTransform::Identity;
	
	UPROPERTY(EditAnywhere, Category = "Patrol|Random")
	bool bRandom = false;
	
	UPROPERTY(EditAnywhere, Category = "Patrol|Random")
	float RandomRange = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Patrol|Search")
	UAnimMontage* SearchMontage = nullptr;

private:
	UPROPERTY()
	ACEnemy_AI* Owner = nullptr;
	UPROPERTY()
	UCAnimInstance* AnimInstance = nullptr;
	UPROPERTY()
	UCMovementComponent* MovementComponent = nullptr;

	bool bPlayingSearchMontage = false;
	
public:
	FORCEINLINE ACPatrolPath* GetPatrolPath() const { return PatrolPath; }
	FORCEINLINE FTransform GetPatrolPoint() const { return PatrolPoint; }

	FORCEINLINE bool GetRandom() const { return bRandom; }
	FORCEINLINE float GetRandomRange() const { return RandomRange; }
	
	FORCEINLINE bool IsPlayingSearchMontage() const { return bPlayingSearchMontage; }

#if DEBUG_AI_Patrol
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
