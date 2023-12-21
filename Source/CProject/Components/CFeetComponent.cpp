#include "Components/CFeetComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/CheckMacros.h"
#include "DebugHeader.h"
#include "MyDebugger/DebuggerComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

#if DEBUG_FEET
	DrawDebug = EDrawDebugTrace::ForOneFrame;
#else
	DrawDebug = EDrawDebugTrace::None;
#endif
}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

#if DEBUG_FEET
	if(UDebuggerComponent* Debugger = GetOwner()->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
	
}

void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float leftDistance, rightDistance;
	FRotator leftRotation, rightRotation;

	Trace(LeftSocketName, leftDistance, leftRotation);
	Trace(RightSocketName, rightDistance, rightRotation);

	UKismetMathLibrary::ClampAngle(leftRotation.Roll, -MaxRoll, MaxRoll);
	UKismetMathLibrary::ClampAngle(leftRotation.Pitch, -MaxPitch, MaxPitch);
	UKismetMathLibrary::ClampAngle(rightRotation.Roll, -MaxRoll, MaxRoll);
	UKismetMathLibrary::ClampAngle(rightRotation.Pitch, -MaxPitch, MaxPitch);

	const float PelvisOffset = FMath::Min(leftDistance, rightDistance);
	FeetData.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(FeetData.PelvisDistance.Z, PelvisOffset, DeltaTime, InterpSpeed);

	FeetData.LeftDistance.X = UKismetMathLibrary::FInterpTo(FeetData.LeftDistance.X, (leftDistance - PelvisOffset), DeltaTime, InterpSpeed);
	FeetData.RightDistance.X = UKismetMathLibrary::FInterpTo(FeetData.RightDistance.X, -(rightDistance - PelvisOffset), DeltaTime, InterpSpeed);

#if DEBUG_FEET
	
#endif
	
}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation) const
{
	OutDistance = 0;
	OutRotation = FRotator::ZeroRotator;

	const FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);

	const float CharacterCenter_Z = OwnerCharacter->GetActorLocation().Z;
	const float Target_Z = CharacterCenter_Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;

	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, CharacterCenter_Z + TraceDistance);
	FVector End = FVector(SocketLocation.X, SocketLocation.Y, Target_Z);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerCharacter);
	
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, true, IgnoreActors, DrawDebug, HitResult, true);

	CheckFalse(HitResult.bBlockingHit);

	const float Length = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
	OutDistance = Length + OffsetDistance - TraceDistance;

	const float Roll = UKismetMathLibrary::DegAtan2(HitResult.Normal.Y, HitResult.Normal.Z);
	const float Pitch = -UKismetMathLibrary::DegAtan2(HitResult.Normal.X, HitResult.Normal.Z);

	OutRotation = FRotator(Pitch, 0, Roll);
}

#if DEBUG_FEET
bool UCFeetComponent::IsDebugEnable()
{
	return true;
}
FDebugInfo UCFeetComponent::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = static_cast<int>(DEBUG_NUMS::FEET);
	Info.Data.Add({"Left Distance: " + FeetData.LeftDistance.ToString(), FColor::Cyan});
	Info.Data.Add({"Right Distance: " + FeetData.RightDistance.ToString(), FColor::Cyan});
	Info.Data.Add({"Pelvis Distance: " + FeetData.PelvisDistance.ToString(), FColor::Cyan});
	Info.Data.Add({"Left Rotation: " + FeetData.LeftRotation.ToString(), FColor::Green});
	Info.Data.Add({"Right Rotation: " + FeetData.RightRotation.ToString(), FColor::Green});

	return Info;
}
#endif