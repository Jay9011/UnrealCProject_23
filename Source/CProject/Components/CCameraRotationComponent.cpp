#include "Components/CCameraRotationComponent.h"

#include "GameFramework/GameSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CheckMacros.h"

UCCameraRotationComponent::UCCameraRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCCameraRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	// 미리 정의된 함수를 바인딩
	{
		FOnTimelineFloat CameraRotationTimelineCallback;
		CameraRotationTimelineCallback.BindUFunction(this, FName("CameraRotationTimelineCallback"));
		const TWeakObjectPtr<UCurveFloat> CurveFloat = NewObject<UCurveFloat>(this, FName("CurveFloat"));
		CameraRotationTimeline.AddInterpFloat(CurveFloat.Get(), CameraRotationTimelineCallback, FName("Default"), FName("Default"));

		FOnTimelineEvent CameraRotationTimelineFinishedCallback;
		CameraRotationTimelineFinishedCallback.BindUFunction(this, FName("CameraRotationTimelineFinishedCallback"));
		CameraRotationTimeline.SetTimelineFinishedFunc(CameraRotationTimelineFinishedCallback);
	}
}

void UCCameraRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CameraRotationTimeline.TickTimeline(DeltaTime);
}

void UCCameraRotationComponent::Play(const FRotator& InTargetRotator, float InMultiplier, float InRate, UCurveFloat* InFloatCurve)
{
	TargetRotator = InTargetRotator;
	CameraRotationMultiplier = InMultiplier;
	
	if (InFloatCurve != nullptr)
	{
		CameraRotationTimeline.SetFloatCurve(InFloatCurve, "Default");
	}

	CameraRotationTimeline.SetPlayRate(InRate);
	CameraRotationTimeline.PlayFromStart();
}

void UCCameraRotationComponent::Stop()
{
	CameraRotationTimeline.Stop();
}

void UCCameraRotationComponent::CameraRotationTimelineCallback(float Value)
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	CheckNull(Pawn);

	AController* Controller = Pawn->GetController();
	FRotator Rotation = Controller->GetControlRotation();

	float Yaw = UKismetMathLibrary::RInterpTo(Rotation, TargetRotator, GetWorld()->GetDeltaSeconds(), Value * CameraRotationMultiplier).Yaw;

	Controller->SetControlRotation(FRotator(Rotation.Pitch, Yaw, Rotation.Roll));
}

void UCCameraRotationComponent::CameraRotationTimelineFinishedCallback()
{
	if (OnCameraRotationFinished.IsBound())
	{
		OnCameraRotationFinished.Broadcast();
		OnCameraRotationFinished.Clear();
	}
}

