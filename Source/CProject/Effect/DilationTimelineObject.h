#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UObject/NoExportTypes.h"
#include "DilationTimelineObject.generated.h"

/**
 * @brief Timeline으로 Dilation 효과를 관리하는 UObject
 */
UCLASS()
class CPROJECT_API UDilationTimelineObject : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay();
	void TickTimeline(float DeltaTime);
	void StartEffect(ACharacter* InOwnerCharacter, float InTargetDilation, UCurveFloat* InCurve, TArray<AActor*>& InIgnoreActors, float InRate = 1.0f);
	void StopEffect();

private:
	UFUNCTION()
	void OnProgress(float Value);
	UFUNCTION()
	void OnFinished();
	
private:
	void BindingInterpFunc();
	void BindingFinishedFunc();

private:
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;
	
	FTimeline Timeline;

	float TargetDilation = 0.f;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
};
