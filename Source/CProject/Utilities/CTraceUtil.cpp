#include "Utilities/CTraceUtil.h"

#include "UDirectionalUtilities.h"
#include "Character/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

#define SELF_DEBUG false	

bool UCTraceUtil::TraceForwardNearEnemyByProfile(ACharacter* InOwnerCharacter, float InLength,
                                                 float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResult)
{
	FVector Start, End;
	UCPositionUtil::GetScreenCenterLocationWithACharacter(InOwnerCharacter, InLength, InRadius, Start, End);

	TArray<FHitResult> HitResult;
	if(!TraceForwardOtherGroupByProfile(InOwnerCharacter, Start, End, InRadius, InProfile, InIgnoreActors, HitResult))
		return false;

	float MinDistance = TNumericLimits<float>::Max();
	for (const FHitResult& Hit : HitResult)
	{
		FVector HitLocation = Hit.Location;
		FVector OwnerLocation = InOwnerCharacter->GetActorLocation();
		float Distance = FVector::DistSquared(HitLocation, OwnerLocation);

		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			OutHitResult = Hit;
		}
	}

	return true;
}

bool UCTraceUtil::TraceForwardOtherGroupByProfile(ACharacter* InOwnerCharacter, FVector InStart,
                                                  FVector InEnd, float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors, TArray<FHitResult>&
                                                  OutHitResults)
{
	EDrawDebugTrace::Type DebugTrace =
#if SELF_DEBUG == true
		EDrawDebugTrace::ForDuration;
#else
		EDrawDebugTrace::None;
#endif

	TArray<FHitResult> HitResult;
	UKismetSystemLibrary::SphereTraceMultiByProfile(InOwnerCharacter->GetWorld(), InStart, InEnd, InRadius, InProfile, false, InIgnoreActors, DebugTrace, HitResult, true);

	for (const FHitResult& Hit : HitResult)
	{
		// 서로 다른 그룹인지 체크
		ACBaseCharacter* OwnerCharacter = Cast<ACBaseCharacter>(InOwnerCharacter);
		ACBaseCharacter* HitCharacter = Cast<ACBaseCharacter>(Hit.GetActor());
		if (OwnerCharacter == nullptr || HitCharacter == nullptr)
			continue;

		if(OwnerCharacter->GetTeamID() != HitCharacter->GetTeamID())
		{
			OutHitResults.Add(Hit);
		}
	}

	return OutHitResults.Num() > 0;
}
