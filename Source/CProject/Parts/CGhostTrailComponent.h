#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "CGhostTrailComponent.generated.h"

class ACGhostTrail;

USTRUCT()
struct FGhostTrailData
{
	GENERATED_BODY()
public:
	int32 Count = 0;
	bool bUse = false;
	
	UPROPERTY()
	ACGhostTrail* GhostTrail = nullptr;
public:
	bool operator==(const FGhostTrailData& InOther) const
	{
		return GhostTrail == InOther.GhostTrail;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCGhostTrailComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCGhostTrailComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** 단일 소환 */
	void Show(const FTransform& InTransform);
	/** Actor의 위치에 반복 소환 */
	void Show(const AActor* InActor, const float Interval);
	/*
	 * Timer 실행중에 고스트 트레일을 중단한다.
	 */
	void Stop();

private:
	void CleanupPool(const int InCount);
	void AddTrailToPool(const FTransform& InTransform);
	void ShowAndUpdateTrailData(FGhostTrailData& InTrailData, const FTransform& InTransform);
	void SetupTrailData(FGhostTrailData& InTrailData, const FTransform& InTransform);
	void StartTrailTimer(FGhostTrailData& InTrailData);

private:
	TDoubleLinkedList<FGhostTrailData> GhostTrailsPool;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	TSubclassOf<ACGhostTrail> GhostTrailActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	FColor GhostTrailColor = FColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	float DelayDestroyTime = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	UCurveFloat* OpacityCurve = nullptr;
	
	int32 SweepCount = 10;
	
private:
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

	FTimerHandle RepeatedTimerHandle;
};
