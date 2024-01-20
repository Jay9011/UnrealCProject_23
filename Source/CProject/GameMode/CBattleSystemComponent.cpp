#include "GameMode/CBattleSystemComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/CAIController.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "GameFramework/Character.h"

UCBattleSystemComponent::UCBattleSystemComponent()
{
}

void UCBattleSystemComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCBattleSystemComponent::RegisterAIController(ACAIController* InAIController)
{
	RegisteredAIControllers.Add(InAIController);
}

void UCBattleSystemComponent::UnregisterAIController(ACAIController* InAIController)
{
	RegisteredAIControllers.Remove(InAIController);
}

void UCBattleSystemComponent::UpdateAttackingAI()
{
	if (RegisteredAIControllers.Num() == 0)
		return;

	AttackingAI = 0;

	// // 무효한 AI가 발견되면 제거하기 위해 저장하는 리스트
	// TArray<TWeakObjectPtr<ACAIController>> RemoveList;
	//
	// // 현재 공격 중인 AI의 수를 계산한다.
	// for (TWeakObjectPtr<ACAIController> AIController : RegisteredAIControllers)
	// {
	// 	if (IsInvalidatedAI(AIController))
	// 	{
	// 		RemoveList.Add(AIController);
	// 		continue;
	// 	}
	//
	// 	if(AIController->GetBlackboardComponent()->GetValueAsBool(CanAttackKey))
	// 		AttackingAI++;
	// }

	// 기존 공격중인 AI를 무효화 한다.
	for (TWeakObjectPtr<ACAIController> AIController : RegisteredAIControllers)
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(CanAttackKey, false);
	}
	
	// 만약, 공격중인 AI의 수가 부족하다면, 가까운 AI가 공격하도록 한다.
	// while (AttackingAI < MaxAttackingAI)
	// {
	// 	TWeakObjectPtr<ACAIController> ClosestAIController = nullptr;
	// 	
	// 	// 만약 가까운 AI를 찾지 못했다면, 더 이상 AI를 찾지 않는다.
	// 	if(!FindClosestAI(ClosestAIController))
	// 		break;
	// 		
	// 	if (ClosestAIController.IsValid())
	// 	{
	// 		ClosestAIController->GetBlackboardComponent()->SetValueAsBool(CanAttackKey, true);
	// 		AttackingAI++;
	// 	}
	// }

	// 현재 설정된 MaxAttackingAI만큼 힙에서 AI를 꺼내서 공격하도록 한다.
	SortAIControllers();
	int MaxNum = FMath::Min(MaxAttackingAI, DistanceAIControllers.Num());
	for (int32 i = 0; i < MaxNum; ++i)
	{
		if (DistanceAIControllers[i].AIController.IsValid())
		{
			DistanceAIControllers[i].AIController->GetBlackboardComponent()->SetValueAsBool(CanAttackKey, true);
			AttackingAI++;
		}
	}
}

bool UCBattleSystemComponent::IsInvalidatedAI(TWeakObjectPtr<ACAIController>& InAIController)
{
	if (InAIController.IsValid() == false)
	{
		return true;
	}
		
	UBlackboardComponent* Blackboard = InAIController->GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		return true;
	}

	return false;
}

void UCBattleSystemComponent::CleanList()
{
	TArray<TWeakObjectPtr<ACAIController>> RemoveList;
	for (TWeakObjectPtr<ACAIController> Controller : RegisteredAIControllers)
	{
		if (Controller.IsValid() == false)
		{
			RemoveList.Add(Controller);
			continue;
		}

		UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
		if (Blackboard == nullptr)
		{
			RemoveList.Add(Controller);
			continue;
		}
	}

	CleanList(RemoveList);
}

void UCBattleSystemComponent::CleanList(TArray<TWeakObjectPtr<ACAIController>>& InRemoveAIController)
{
	for (TWeakObjectPtr<ACAIController> Controller : InRemoveAIController)
	{
		RegisteredAIControllers.Remove(Controller);
	}
}

bool UCBattleSystemComponent::FindClosestAI(TWeakObjectPtr<ACAIController>& OutClosestAIController)
{
	bool bResult = false;
	float ClosestDistance = FLT_MAX;
	for (TWeakObjectPtr<ACAIController> Controller : RegisteredAIControllers)
	{
		if (IsInvalidatedAI(Controller))
			continue;

		UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
		if (Blackboard->GetValueAsBool(CanAttackKey))
			continue;

		ACharacter* Target = Cast<ACharacter>(Blackboard->GetValueAsObject(Controller->GetBehaviorComponent()->GetTargetKey()));
		if (Target == nullptr)
			continue;

		float DistanceSq = FVector::DistSquared(Target->GetActorLocation(), Controller->GetPawn()->GetActorLocation());
		
		if (DistanceSq < ClosestDistance)
		{
			ClosestDistance = DistanceSq;
			OutClosestAIController = Controller;
			bResult = true;
		}
	}

	return bResult;
}

void UCBattleSystemComponent::SortAIControllers()
{
	DistanceAIControllers.Empty();
	DistanceAIControllers.Reserve(RegisteredAIControllers.Num());

	for (TWeakObjectPtr<ACAIController> Controller : RegisteredAIControllers)
	{
		if (IsInvalidatedAI(Controller))
			continue;

		UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
		if (Blackboard->GetValueAsBool(CanAttackKey))
			continue;

		ACharacter* Target = Cast<ACharacter>(Blackboard->GetValueAsObject(Controller->GetBehaviorComponent()->GetTargetKey()));
		if (Target == nullptr)
			continue;

		float DistanceSq = FVector::DistSquared(Target->GetActorLocation(), Controller->GetPawn()->GetActorLocation());
		
		FDistanceAIController DistanceAIController;
		DistanceAIController.Distance = DistanceSq;
		DistanceAIController.AIController = Controller;
		
		DistanceAIControllers.Add(DistanceAIController);
	}
	
	DistanceAIControllers.Sort();
}
