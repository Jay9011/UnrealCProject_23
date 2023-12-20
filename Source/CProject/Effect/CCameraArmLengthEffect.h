#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "UObject/NoExportTypes.h"
#include "CCameraArmLengthEffect.generated.h"

class USpringArmComponent;
/**
 * @brief Timeline으로 카메라 암 길이를 관리하는 UObject
 */
UCLASS()
class CPROJECT_API UCCameraArmLengthEffect : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay();
	void TickTimeline(float DeltaTime);
	void StartEffect(USpringArmComponent* InSpringArm, float InTargetArmLength, UCurveFloat* InCurve);
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
	USpringArmComponent* SpringArm;

	FTimeline Timeline;

	float BackupArmLength = 0.f;
	float TargetArmLength = 0.f;
};
