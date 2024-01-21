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
	// Interval이 0보다 작거나 같으면 단일 소환
	if (Interval <= 0.f)
	{
		Show(InActor->GetTransform());
		return;
	}
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, InActor]()
	{
		Show(InActor->GetTransform());
	});

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(RepeatedTimerHandle, TimerDelegate, Interval, true, 0.f);
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
		SetupTrailData(TrailData, InTransform);
		StartTrailTimer(TrailData);
		TrailData.GhostTrail->FinishSpawning(InTransform);
		TrailData.GhostTrail->Start(InTransform);
		GhostTrailsPool.AddTail(TrailData);
	}
}

void UCGhostTrailComponent::EndUseTrail(FGhostTrailData& InTrailData)
{
	auto Node = GhostTrailsPool.FindNode(InTrailData);
	Node->GetValue().bUse = false;
	MoveTrailHeadToPool(Node);
}

void UCGhostTrailComponent::MoveTrailHeadToPool(TDoubleLinkedList<FGhostTrailData>::TDoubleLinkedListNode* InNode)
{
	GhostTrailsPool.RemoveNode(InNode, false);
	GhostTrailsPool.AddHead(InNode);
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
		auto Node = GhostTrailsPool.FindNode(InTrailData);
		Node->GetValue().bUse = false;
		GhostTrailsPool.RemoveNode(Node, false);
		GhostTrailsPool.AddHead(Node);
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
