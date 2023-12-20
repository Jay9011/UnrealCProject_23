#include "Effect/CCameraArmLengthEffect.h"

#include "GameFramework/SpringArmComponent.h"
#include "Utilities/CheckMacros.h"

void UCCameraArmLengthEffect::BeginPlay()
{
	BindingInterpFunc();
	BindingFinishedFunc();
}

void UCCameraArmLengthEffect::TickTimeline(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCCameraArmLengthEffect::StartEffect(USpringArmComponent* InSpringArm, float InTargetArmLength,
	UCurveFloat* InCurve)
{
	if (Timeline.IsPlaying())
	{
		StopEffect();
	}

	SpringArm = InSpringArm;
	TargetArmLength = InTargetArmLength;
	BackupArmLength = SpringArm->TargetArmLength;

	Timeline.SetFloatCurve(InCurve, FName("Default"));
	Timeline.PlayFromStart();
}

void UCCameraArmLengthEffect::StopEffect()
{
	Timeline.Stop();
	OnFinished();
}

void UCCameraArmLengthEffect::OnProgress(float Value)
{
	float ArmLength = BackupArmLength + Value * (TargetArmLength - BackupArmLength);

	SpringArm->TargetArmLength = ArmLength;
}

void UCCameraArmLengthEffect::OnFinished()
{
	SpringArm->TargetArmLength = BackupArmLength;
}

void UCCameraArmLengthEffect::BindingInterpFunc()
{
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "OnProgress");
	const TWeakObjectPtr<UCurveFloat> CurveFloat = NewObject<UCurveFloat>(this, FName("CurveFloat"));
	Timeline.AddInterpFloat(CurveFloat.Get(), OnTimelineCallback, FName("Default"), FName("Default"));
}

void UCCameraArmLengthEffect::BindingFinishedFunc()
{
	FOnTimelineEvent OnTimelineFinishedCallback;
	OnTimelineFinishedCallback.BindUFunction(this, "OnFinished");
	Timeline.SetTimelineFinishedFunc(OnTimelineFinishedCallback);
}
