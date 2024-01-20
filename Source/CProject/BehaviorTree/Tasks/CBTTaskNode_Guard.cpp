#include "BehaviorTree/Tasks/CBTTaskNode_Guard.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CGuardComponent.h"
#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CtrlActions/CGuardAction.h"

UCBTTaskNode_Guard::UCBTTaskNode_Guard()
{
	NodeName = "Guard";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(AIController->GetPawn());
	CheckNullResult(AIController, EBTNodeResult::Failed);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(Enemy_AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Failed);

	UCGuardComponent* GuardComponent = Cast<UCGuardComponent>(Enemy_AI->GetComponentByClass(UCGuardComponent::StaticClass()));
	CheckNullResult(GuardComponent, EBTNodeResult::Failed);

	UCGuardAction* GuardAction = Cast<UCGuardAction>(WeaponComponent->GetCtrlAction());
	CheckNullResult(GuardAction, EBTNodeResult::Failed);

	UCStateComponent* StateComponent = Cast<UCStateComponent>(Enemy_AI->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNullResult(StateComponent, EBTNodeResult::Failed);

	if (bDoGuard)
	{
		if (GuardComponent->IsUnGuard() && StateComponent->IsIdleMode())
		{
			GuardAction->Pressed();
			return EBTNodeResult::InProgress;
		}
	}
	else
	{
		if (!GuardComponent->IsUnGuard())
		{
			GuardAction->Released();
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Guard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(AIController->GetPawn());
	if (Enemy_AI == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}

	UCGuardComponent* GuardComponent = Cast<UCGuardComponent>(Enemy_AI->GetComponentByClass(UCGuardComponent::StaticClass()));
	if (GuardComponent == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}

	if (!GuardComponent->IsUnGuard())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}

EBTNodeResult::Type UCBTTaskNode_Guard::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(AIController->GetPawn());
	CheckNullResult(AIController, EBTNodeResult::Aborted);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(Enemy_AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, EBTNodeResult::Aborted);

	UCGuardAction* GuardAction = Cast<UCGuardAction>(WeaponComponent->GetCtrlAction());
	CheckNullResult(GuardAction, EBTNodeResult::Aborted);

	GuardAction->Released();
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}
