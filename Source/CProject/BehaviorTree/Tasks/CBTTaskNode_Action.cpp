#include "BehaviorTree/Tasks/CBTTaskNode_Action.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CComboState.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"
#include "Weapons/IComboState.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = TEXT("Action");

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(Enemy->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(Weapon, EBTNodeResult::Failed);

	IIComboState* IComboState = Cast<IIComboState>(Weapon->GetDoAction());
	if (bUseCombo && IComboState != nullptr)
	{
		if (bUseRandomCombo)
		{
			RandomComboIndex = FMath::RandRange(0, Weapon->GetDoAction()->GetActionDataCount() - 1);
		}
		else
		{
			RandomComboIndex = Weapon->GetDoAction()->GetActionDataCount() - 1;
		}
	}
	
	Weapon->DoAction();
	
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(Enemy->GetComponentByClass(UCWeaponComponent::StaticClass()));
	UCStateComponent* State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	if (State == nullptr)
	{
		FinishLatentAbort(OwnerComp);
	}

	bool bCheck = true;
	bCheck &= (State->IsIdleMode());
	bCheck &= (Weapon->GetDoAction()->IsInAction() == false);

	// 만약, Action이 끝났다고 판단되면 종료
	if (bCheck)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		return;
	}

	// Action이 끝나지 않았고, Combo를 사용한다면
	if (bUseCombo)
	{
		IIComboState* IComboState = Cast<IIComboState>(Weapon->GetDoAction());
		UCComboState* ComboState = Cast<UCComboState>(IComboState->GetComboState());

		// RandomCombo가 끝났다면, Action이 종료될 때까지 기다린다.
		if (bUseRandomCombo && (ComboState->GetIndex() >= RandomComboIndex))
		{
			return;
		}

		Weapon->DoAction();
	}
}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());

	UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(Enemy->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(Weapon, EBTNodeResult::Aborted);

	UCAIBehaviorComponent* AIBehavior = Controller->GetBehaviorComponent();
	CheckNullResult(AIBehavior, EBTNodeResult::Aborted);

	// Hit에 의해 Action이 중단되었다면,
	if (AIBehavior->IsHitted())
	{
		bool bBeginAction = Weapon->GetDoAction()->IsBeginAction();
		if (bBeginAction == false)
		{
			Weapon->GetDoAction()->Begin_DoAction();
		}

		Weapon->GetDoAction()->End_DoAction();
		return EBTNodeResult::Aborted;
	}
	
	// 그게 아니라 다른 조건에 의해 종료되었다면, 현재까지의 Action을 끝내고 종료한다.
	return EBTNodeResult::Succeeded;
}
