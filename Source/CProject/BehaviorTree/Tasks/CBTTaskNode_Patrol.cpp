#include "BehaviorTree/Tasks/CBTTaskNode_Patrol.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Components/AI/CAIPatrolComponent.h"
#include "Components/AI/Parts/CPatrolPath.h"
#include "Utilities/CheckMacros.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCAIBehaviorComponent* Behavior = Cast<UCAIBehaviorComponent>(Enemy_AI->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	CheckNullResult(Behavior, EBTNodeResult::Failed);
	
	UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy_AI->GetComponentByClass(UCAIPatrolComponent::StaticClass()));
	CheckNullResult(Patrol, EBTNodeResult::Failed);

	// PatrolPath가 있는 경우 PatrolPath의 위치로 이동
	if (Patrol->GetPatrolPath() != nullptr)
	{
		FVector NextLocation = Patrol->GetPatrolPath()->GetMoveTo();
		Behavior->SetPatrolLocation(NextLocation);
		DrawDebug(Enemy_AI->GetWorld(), NextLocation);

		return EBTNodeResult::InProgress;
	}
	
	// PatrolPath가 없는 경우, PatrolComponent의 Random이 true라면, 랜덤 위치로 이동
	bool bFound = false;
	if (Patrol->GetRandom())
	{
		FVector ActorLocation = Enemy_AI->GetActorLocation();

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy_AI->GetWorld());
		CheckNullResult(NavSys, EBTNodeResult::Failed);

		FNavLocation NextPoint(ActorLocation);
		for (uint8 i = 0; i < MaxAttempts; ++i)
		{
			if (NavSys->GetRandomPointInNavigableRadius(ActorLocation, Patrol->GetRandomRange(), NextPoint))
			{
				bFound = true;
				break;
			}
		}
		if (bFound)
		{
			Behavior->SetPatrolLocation(NextPoint.Location);
			DrawDebug(Enemy_AI->GetWorld(), NextPoint.Location);
			
			return EBTNodeResult::InProgress;
		}
	}
	// PatrolPath가 없는 경우, PatrolComponent의 Random이 false라면, 기존 위치로 이동
	else
	{
		Behavior->SetPatrolLocation(Patrol->GetPatrolPoint().GetTranslation());
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(Controller->GetPawn());
	UCAIBehaviorComponent* Behavior = Cast<UCAIBehaviorComponent>(Enemy_AI->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (Behavior == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return;
	}

	FVector NextLocation = Behavior->GetPatrolLocation();
	EPathFollowingRequestResult::Type Result = Controller->MoveToLocation(NextLocation, AcceptanceDistance, false);

	switch (Result)
	{
	case EPathFollowingRequestResult::Failed:
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy_AI->GetComponentByClass(UCAIPatrolComponent::StaticClass()));
			
			if (Patrol->GetPatrolPath())
			{
				Patrol->GetPatrolPath()->UpdateIndex();
			}
			// Patrol Path 없이 원래 위치로 돌아온 경우
			else if (Patrol->GetRandom() == false)
			{
				InterpRotation(OwnerComp, DeltaSeconds);
				if (!Enemy_AI->GetActorRotation().Equals(Patrol->GetPatrolPoint().GetRotation().Rotator(), 1.f))
				{
					return;
				}
			}
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		break;
	}
}

EBTNodeResult::Type UCBTTaskNode_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	Controller->StopMovement();
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBTTaskNode_Patrol::DrawDebug(const UWorld* InWorld, const FVector& InLocation)
{
	if (bDebugMode)
	{
		DrawDebugSphere(InWorld, InLocation, 10.f, 10, FColor::Green, false, 3.f);
	}
}

void UCBTTaskNode_Patrol::InterpRotation(UBehaviorTreeComponent& OwnerComp, float DeltaSeconds)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy_AI->GetComponentByClass(UCAIPatrolComponent::StaticClass()));
	if (Patrol == nullptr)
		return;

	FRotator CurrentRotation = Enemy_AI->GetActorRotation();
	FRotator TargetRotation = Patrol->GetPatrolPoint().GetRotation().Rotator();

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 5.f);
	Enemy_AI->SetActorRotation(NewRotation);
}
