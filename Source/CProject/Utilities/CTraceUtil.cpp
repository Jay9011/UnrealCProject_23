#include "Utilities/CTraceUtil.h"

#include "UDirectionalUtilities.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

bool UCTraceUtil::TraceForwardNearEnemyByProfile(ACharacter* InOwnerCharacter, float InLength,
                                                 float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResult,
                                                 bool bDebug, EDrawDebugTrace::Type DebugTrace, float Duration)
{
	if (!bDebug)
		DebugTrace = EDrawDebugTrace::None;
	
	FVector Start, End;
	UCPositionUtil::GetScreenCenterLocationWithACharacter(InOwnerCharacter, InLength, InRadius, Start, End);

	TArray<FHitResult> HitResult;
	if(!TraceForwardOtherGroupByProfile(InOwnerCharacter, Start, End, InRadius, InProfile, InIgnoreActors, HitResult, bDebug, DebugTrace, Duration))
		return false;

	float MinDistance = TNumericLimits<float>::Max();
	for (const FHitResult& Hit : HitResult)
	{
		FVector HitLocation = Hit.Location;
		FVector OwnerLocation = InOwnerCharacter->GetActorLocation();
		const float Distance = FVector::DistSquared(HitLocation, OwnerLocation);

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			OutHitResult = Hit;
		}
	}

	return true;
}

bool UCTraceUtil::TraceForwardOtherGroupByProfile(ACharacter* InOwnerCharacter, FVector InStart,
                                                  FVector InEnd, float InRadius, const FName& InProfile,
                                                  const TArray<AActor*>& InIgnoreActors, TArray<FHitResult>& OutHitResults,
                                                  bool bDebug, EDrawDebugTrace::Type DebugTrace, float Duration)
{
	if (!bDebug)
		DebugTrace = EDrawDebugTrace::None;
	
	TArray<FHitResult> HitResult;
	UKismetSystemLibrary::SphereTraceMultiByProfile(InOwnerCharacter->GetWorld(), InStart, InEnd, InRadius, InProfile, false, InIgnoreActors, DebugTrace, HitResult, true, FLinearColor::Red, FLinearColor::Green, Duration);

	for (const FHitResult& Hit : HitResult)
	{
		// 서로 다른 그룹인지 체크
		const ACBaseCharacter* OwnerCharacter = Cast<ACBaseCharacter>(InOwnerCharacter);
		const ACBaseCharacter* HitCharacter = Cast<ACBaseCharacter>(Hit.GetActor());
		if (OwnerCharacter == nullptr || HitCharacter == nullptr)
			continue;

		if(OwnerCharacter->GetTeamID() != HitCharacter->GetTeamID())
		{
			OutHitResults.Add(Hit);
		}
	}

	return OutHitResults.Num() > 0;
}

bool UCTraceUtil::SingleTraceSphereOtherGroupForObjects(ACharacter* InOwnerCharacter, float InRadius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
														const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResults,
														bool bDebug, EDrawDebugTrace::Type DebugTrace, float Duration)
{
	if (!bDebug)
		DebugTrace = EDrawDebugTrace::None;
	
	TArray<FHitResult> HitResults;
	if(!UKismetSystemLibrary::SphereTraceMultiForObjects(InOwnerCharacter->GetWorld(), InOwnerCharacter->GetActorLocation(), InOwnerCharacter->GetActorLocation(), InRadius, ObjectTypes, false, InIgnoreActors, DebugTrace, HitResults, true
		, FLinearColor::Red, FLinearColor::Green, Duration))
		return false;

	for (const FHitResult& result : HitResults)
	{
		// 서로 다른 그룹인지 체크
		const ACBaseCharacter* OwnerCharacter = Cast<ACBaseCharacter>(InOwnerCharacter);
		const ACBaseCharacter* HitCharacter = nullptr;
		
		// result.GetActor()가 캐릭터가 아니고 Owner가 또 존재하면 Owner로 변환을 시도해본다.
		AActor* Owner = result.GetActor();
		while(Owner != nullptr)
		{
			HitCharacter = Cast<ACBaseCharacter>(Owner);
			if (HitCharacter != nullptr)
				break;

			Owner = Owner->GetOwner();
		}
		
		if (OwnerCharacter == nullptr || HitCharacter == nullptr)
			continue;

		if (OwnerCharacter->GetTeamID() != HitCharacter->GetTeamID())
		{
			OutHitResults = result;
			return true;
		}
	}
	
	return false;
}

bool UCTraceUtil::SingleTraceSphereOtherGroupTraceChannel(ACharacter* InOwnerCharacter, float InRadius,
	const TEnumAsByte<ETraceTypeQuery> TraceChannel, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResults, bool bDebug,
	EDrawDebugTrace::Type DebugTrace, float Duration)
{
	if (!bDebug)
		DebugTrace = EDrawDebugTrace::None;

	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(InOwnerCharacter->GetWorld(), InOwnerCharacter->GetActorLocation(), InOwnerCharacter->GetActorLocation(), InRadius, TraceChannel, false, InIgnoreActors, DebugTrace, HitResults, true
		, FLinearColor::Red, FLinearColor::Green, Duration);
	
	for (const FHitResult& result : HitResults)
	{
		// 서로 다른 그룹인지 체크
		const ACBaseCharacter* OwnerCharacter = Cast<ACBaseCharacter>(InOwnerCharacter);
		const ACBaseCharacter* HitCharacter = nullptr;
		
		// result.GetActor()가 캐릭터가 아니고 Owner가 또 존재하면 Owner로 변환을 시도해본다.
		AActor* Owner = result.GetActor();
		while(Owner != nullptr)
		{
			HitCharacter = Cast<ACBaseCharacter>(Owner);
			if (HitCharacter != nullptr)
				break;

			Owner = Owner->GetOwner();
		}
		
		if (OwnerCharacter == nullptr || HitCharacter == nullptr)
			continue;

		if (OwnerCharacter->GetTeamID() != HitCharacter->GetTeamID())
		{
			OutHitResults = result;
			return true;
		}
	}
	
	return false;
}
