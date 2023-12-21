#include "CNeckComponent.h"

#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "DebugHeader.h"
#include "MyDebugger/DebuggerComponent.h"

UCNeckComponent::UCNeckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCNeckComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

#if DEBUG_HEAD
	if(UDebuggerComponent* Debugger = GetOwner()->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
}

void UCNeckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator HeadRotation = GetHeadRotation();

	HeadRotation.Pitch = FMath::ClampAngle(HeadRotation.Pitch, MinPitch, MaxPitch);
	HeadRotation.Yaw = FMath::ClampAngle(HeadRotation.Yaw, MinYaw, MaxYaw);
	
	EffectorRotation = FMath::RInterpTo(EffectorRotation, HeadRotation, DeltaTime, InterpSpeed);

	// ControlRotationYaw를 사용하면 Yaw에 대해 회전 값을 주면 안된다.
	if (OwnerCharacter && OwnerCharacter->bUseControllerRotationYaw)
	{
		EffectorRotation.Yaw = 0.f;
	}
}

FRotator UCNeckComponent::GetHeadRotation()
{
	CheckNullResult(OwnerCharacter, FRotator::ZeroRotator);

	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	FRotator ActorRotation = OwnerCharacter->GetActorRotation();

#if DEBUG_HEAD
	DebugControlRotation = ControlRotation;
	DebugActorRotation = ActorRotation;
#endif
	
	return FRotator(ControlRotation.Pitch, ControlRotation.Yaw - ActorRotation.Yaw, 0.f);
}

#if DEBUG_HEAD
bool UCNeckComponent::IsDebugEnable()
{
	return true;
}

FDebugInfo UCNeckComponent::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = static_cast<int>(DEBUG_NUMS::FEET);
	Info.Data.Add({"Neck Rotation: " + EffectorRotation.ToString(), FColor::Cyan});
	Info.Data.Add({"Control Rotation: " + DebugControlRotation.ToString(), FColor::Green});
	Info.Data.Add({"Actor Rotation: " + DebugActorRotation.ToString(), FColor::Green});
	
	return Info;
}
#endif