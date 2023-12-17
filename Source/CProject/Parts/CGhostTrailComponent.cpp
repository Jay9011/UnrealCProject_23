#include "Parts/CGhostTrailComponent.h"

#include "CGhostTrail.h"
#include "GameFramework/Character.h"

UCGhostTrailComponent::UCGhostTrailComponent()
{
}

void UCGhostTrailComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCGhostTrailComponent::Show(const FTransform& InTransform)
{
	TDoubleLinkedList<FGhostTrailData>::TIterator It(GhostTrailsPool.GetHead());
	for (; It; It++)
	{
		if (It.GetNode()->GetValue().bUse)
			continue;

		ShowAndUpdateTrailData(It.GetNode()->GetValue(), InTransform);

		// 사용한 Trail은 Pool 제일 뒤로 보냄
		GhostTrailsPool.RemoveNode(It.GetNode(), false);
		GhostTrailsPool.AddTail(It.GetNode());
		return;
	}

	// Pool에 사용할 수 있는 Trail이 없을 경우
	AddTrailToPool(InTransform);
}

void UCGhostTrailComponent::Show(const AActor* InActor, const float Interval)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, InActor]()
	{
		Show(InActor->GetTransform());
	});

	GetWorld()->GetTimerManager().SetTimer(RepeatedTimerHandle, TimerDelegate, Interval, true);
}

void UCGhostTrailComponent::Stop()
{
	if (!RepeatedTimerHandle.IsValid())
		return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	
	if(TimerManager.IsTimerActive(RepeatedTimerHandle))
	{
		TimerManager.ClearTimer(RepeatedTimerHandle);
	}
}

void UCGhostTrailComponent::CleanupPool(const int InCount)
{
 	for (FGhostTrailData& TrailData : GhostTrailsPool)
	{
		if (TrailData.bUse)
			continue;

		TrailData.Count -= InCount;
		if (TrailData.Count < 0)
		{
			TrailData.GhostTrail->Destroy();
			GhostTrailsPool.RemoveNode(TrailData);
		}
	}
}

void UCGhostTrailComponent::AddTrailToPool(const FTransform& InTransform)
{
	FGhostTrailData TrailData;
	TrailData.GhostTrail = GetWorld()->SpawnActorDeferred<ACGhostTrail>(GhostTrailActorClass, InTransform, GetOwner(), OwnerCharacter, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (TrailData.GhostTrail != nullptr)
	{
		if (GhostTrailsPool.AddTail(TrailData))
		{
			SetupTrailData(TrailData, InTransform);
			TrailData.GhostTrail->FinishSpawning(InTransform);
		
			StartTrailTimer(TrailData);
		}
	}
}

void UCGhostTrailComponent::ShowAndUpdateTrailData(FGhostTrailData& InTrailData, const FTransform& InTransform)
{
	if (InTrailData.GhostTrail->Start(InTransform))
	{
		InTrailData.Count++;
		InTrailData.bUse = true;
	
		StartTrailTimer(InTrailData);
	
		if(InTrailData.Count >= SweepCount)
			CleanupPool(SweepCount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GhostTrail Show Error"));
	}
}

void UCGhostTrailComponent::SetupTrailData(FGhostTrailData& InTrailData, const FTransform& InTransform)
{
	InTrailData.GhostTrail->InitData(GhostTrailColor, OwnerCharacter);
	InTrailData.Count = 1;
	InTrailData.bUse = true;
}

void UCGhostTrailComponent::StartTrailTimer(FGhostTrailData& InTrailData)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, InTrailData]()
	{
		InTrailData.GhostTrail->End();
		GhostTrailsPool.FindNode(InTrailData)->GetValue().bUse = false;
	});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayDestroyTime, false);

	if (OpacityCurve != nullptr)
	{
		float MinTime = 0.f, MaxTime = 0.f;
		OpacityCurve->GetTimeRange(MinTime, MaxTime);
		InTrailData.GhostTrail->SetOpacity(OpacityCurve, FMath::Clamp(DelayDestroyTime - MaxTime, 0.f, DelayDestroyTime));
	}
}
