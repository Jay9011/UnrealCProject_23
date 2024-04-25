#include "BehaviorTree/Tasks/CBTTask_SearchMontage.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Components/AI/CAIPatrolComponent.h"

UCBTTask_SearchMontage::UCBTTask_SearchMontage()
{
	NodeName = TEXT("PlaySearchMontage");
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTask_SearchMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	if (Controller == nullptr || Enemy == nullptr)
		return EBTNodeResult::Failed;

	UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy->GetComponentByClass(UCAIPatrolComponent::StaticClass()));
	if (Patrol == nullptr)
		return EBTNodeResult::Failed;

	Patrol->PlaySearchMontage();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTask_SearchMontage::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());
	
	if (Controller == nullptr || Enemy == nullptr)
		return EBTNodeResult::Failed;

	UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy->GetComponentByClass(UCAIPatrolComponent::StaticClass()));
	if (Patrol == nullptr)
		return EBTNodeResult::Failed;

	Patrol->StopSearchMontage();

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCBTTask_SearchMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCAIPatrolComponent* Patrol = Cast<UCAIPatrolComponent>(Enemy->GetComponentByClass(UCAIPatrolComponent::StaticClass()));

	if (Patrol == nullptr)
		return;

	if (!Patrol->IsPlayingSearchMontage())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}