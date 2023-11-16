#include "Utilities/GTimeController.h"

#include "CheckMacros.h"
#include "GameFramework/Character.h"

void UGTimeController::SetTimeDilation(UWorld* InWorld, float InTimeDilation, float InDuration,
	TArray<AActor*> InIgnoreActors)
{
	if(!InWorld) return;
	CheckTrue(FMath::IsNearlyZero(InTimeDilation));

	bool bIsIgnoreActors = InIgnoreActors.Num() > 0;

	TArray<APawn*> Pawns;
	for (AActor* Actor : InWorld->GetCurrentLevel()->Actors)
	{
		APawn* Pawn = Cast<ACharacter>(Actor);
		if (!!Pawn && (!bIsIgnoreActors || !InIgnoreActors.Contains(Pawn)))
		{
			Pawns.Add(Pawn);
			Pawn->CustomTimeDilation = InTimeDilation;
		}
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([=]()
	{
		for (APawn* Pawn : Pawns)
		{
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
	TArray<AActor*> InIgnoreActors)
{
	SetTimeDilation(InWorld, InTimeDilation, InDuration, InIgnoreActors);
}
