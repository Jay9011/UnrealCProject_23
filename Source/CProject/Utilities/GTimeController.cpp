#include "Utilities/GTimeController.h"

#include "CheckMacros.h"
#include "GameFramework/Character.h"

void UGTimeController::SetTimeDilation(const UWorld* InWorld, float InTimeDilation, float InDuration,
                                       const TArray<AActor*>& InIgnoreActors)
{
	if(!InWorld) return;
	CheckTrue(FMath::IsNearlyZero(InTimeDilation));

	const bool bIsIgnoreActors = InIgnoreActors.Num() > 0;

	TArray<APawn*> Pawns;
	for (AActor* Actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* Pawn = Cast<ACharacter>(Actor);
		if (!!Pawn && (!bIsIgnoreActors || !InIgnoreActors.Contains(Pawn)))
		{
			Pawn->CustomTimeDilation = InTimeDilation;
			Pawns.Add(Pawn);
		}
	}

	
	if (FMath::IsNearlyZero(InDuration))
		return;

	
	/*
	 * InDuration 이 0이 아닐 경우, InDuration 이 지난 후에 TimeDilation을 원래대로 돌려준다.
	 */
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=]()
	{
		for (APawn* Pawn : Pawns)
		{
			if (!IsValid(Pawn))
				continue;
			
			Pawn->CustomTimeDilation = 1.f;
		}
	});

	FTimerHandle TimerHandle;
	InWorld->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, InDuration, false);
}

void UGTimeController::SetTimeDilationOnlyActors(UWorld* InWorld, float InTimeDilation, float InDuration)
{
	SetTimeDilation(InWorld, InTimeDilation, InDuration);
}

void UGTimeController::SetTimeDilationOnlyActorsAddIgnoreActors(UWorld* InWorld, float InTimeDilation, float InDuration,
                                                                TArray<AActor*>& InIgnoreActors)
{
	SetTimeDilation(InWorld, InTimeDilation, InDuration, InIgnoreActors);
}
