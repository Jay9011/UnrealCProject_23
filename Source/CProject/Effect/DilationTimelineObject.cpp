#include "Effect/DilationTimelineObject.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/GTimeController.h"

void UDilationTimelineObject::BeginPlay()
{
	BindingInterpFunc();
	BindingFinishedFunc();
}

void UDilationTimelineObject::TickTimeline(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UDilationTimelineObject::StartEffect(ACharacter* InOwnerCharacter, float InTargetDilation, UCurveFloat* InCurve,
                                          TArray<AActor*>& InIgnoreActors, float InRate)
{
	if (Timeline.IsPlaying())
	{
		StopEffect();
	}
	
	OwnerCharacter = InOwnerCharacter;
	TargetDilation = InTargetDilation;
	IgnoreActors = InIgnoreActors;

	Timeline.SetFloatCurve(InCurve, FName("Default"));
	Timeline.SetPlayRate(InRate);
	Timeline.PlayFromStart();
}

void UDilationTimelineObject::StopEffect()
{
	Timeline.Stop();
	OnFinished();
}

void UDilationTimelineObject::OnProgress(float Value)
{
	float Dilation = TargetDilation + Value * (1.0f - TargetDilation);

	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter->GetWorld(), Dilation);
}

void UDilationTimelineObject::OnFinished()
{
	UGameplayStatics::SetGlobalTimeDilation(OwnerCharacter->GetWorld(), 1.0f);
	IgnoreActors.Empty();
}

void UDilationTimelineObject::BindingInterpFunc()
{
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "OnProgress");
	const TWeakObjectPtr<UCurveFloat> CurveFloat = NewObject<UCurveFloat>(this, FName("CurveFloat"));
	Timeline.AddInterpFloat(CurveFloat.Get(), OnTimelineCallback, FName("Default"), FName("Default"));
}

void UDilationTimelineObject::BindingFinishedFunc()
{
	FOnTimelineEvent OnTimelineFinishedCallback;
	OnTimelineFinishedCallback.BindUFunction(this, "OnFinished");
	Timeline.SetTimelineFinishedFunc(OnTimelineFinishedCallback);
}
