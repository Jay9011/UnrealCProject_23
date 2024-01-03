#include "BehaviorTree/Tasks/CBTTaskNode_Patrol.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/AI/CAIBehaviorComponent.h"
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

	// PatrolPath가 있는 경우 PatrolPath의 위치로 이동
	if (Enemy_AI->GetPatrolPath() != nullptr)
	{
		FVector NextLocation = Enemy_AI->GetPatrolPath()->GetMoveTo();
		Behavior->SetPatrolLocation(NextLocation);

		DrawDebug(Enemy_AI->GetWorld(), NextLocation);

		return EBTNodeResult::InProgress;
	}
	

	// PatrolPath가 없는 경우 랜덤 위치로 이동
	FVector ActorLocation = Enemy_AI->GetActorLocation();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy_AI->GetWorld());
	CheckNullResult(NavSys, EBTNodeResult::Failed);

	FNavLocation NextPoint(ActorLocation);
	while (true)
	{
		if (NavSys->GetRandomPointInNavigableRadius(ActorLocation, RandomRange, NextPoint))
		{
			break;
		}
	}

	Behavior->SetPatrolLocation(NextPoint.Location);

	DrawDebug(Enemy_AI->GetWorld(), NextPoint.Location);

	return EBTNodeResult::InProgress;
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
			if (Enemy_AI->GetPatrolPath())
			{
				Enemy_AI->GetPatrolPath()->UpdateIndex();
			}
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		break;
	}
}

void UCBTTaskNode_Patrol::DrawDebug(const UWorld* InWorld, const FVector& InLocation)
{
	if (bDebugMode)
	{
		DrawDebugSphere(InWorld, InLocation, 10.f, 10, FColor::Green, false, 3.f);
	}
}
