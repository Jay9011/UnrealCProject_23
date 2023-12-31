#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "CCameraRotationComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCameraRotationFinished);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCCameraRotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCCameraRotationComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Play(const FRotator& InTargetRotator, float InMultiplier, float InRate, UCurveFloat* InFloatCurve);
	void Stop();
	bool IsPlaying() const { return CameraRotationTimeline.IsPlaying(); }
	
/* Timeline Function */
private:
	UFUNCTION()
	void CameraRotationTimelineCallback(float Value);
	UFUNCTION()
	void CameraRotationTimelineFinishedCallback();

public:
	FOnCameraRotationFinished OnCameraRotationFinished;
	
private:
	FTimeline CameraRotationTimeline;

	FRotator TargetRotator;
	float CameraRotationMultiplier = 20.0f;
};
