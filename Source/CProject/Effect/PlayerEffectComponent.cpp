#include "Effect/PlayerEffectComponent.h"

#include "CCameraArmLengthEffect.h"
#include "DilationTimelineObject.h"

UPlayerEffectComponent::UPlayerEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	DilationTimelineObject = CreateDefaultSubobject<UDilationTimelineObject>(TEXT("DilationTimelineObject"));
	CameraArmLengthEffect = CreateDefaultSubobject<UCCameraArmLengthEffect>(TEXT("CameraArmLengthEffect"));
}

void UPlayerEffectComponent::BeginPlay()
{
	Super::BeginPlay();
	DilationTimelineObject->BeginPlay();
	CameraArmLengthEffect->BeginPlay();
}

void UPlayerEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DilationTimelineObject != nullptr)
		DilationTimelineObject->TickTimeline(DeltaTime);

	if (CameraArmLengthEffect != nullptr)
		CameraArmLengthEffect->TickTimeline(DeltaTime);
}

void UPlayerEffectComponent::DilationEffect(ACharacter* InOwnerCharacter, const float InTargetDilation,
	TArray<AActor*>& InIgnoreActors)
{
	DilationTimelineObject->StartEffect(InOwnerCharacter, InTargetDilation, DilationCurve, InIgnoreActors);
}

void UPlayerEffectComponent::ArmLengthEffect(USpringArmComponent* InSpringArm, const float InTargetArmLength)
{
	CameraArmLengthEffect->StartEffect(InSpringArm, InTargetArmLength, ArmLengthCurve);
}

