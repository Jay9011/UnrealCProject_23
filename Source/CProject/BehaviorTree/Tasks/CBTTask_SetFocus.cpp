#include "BehaviorTree/Tasks/CBTTask_SetFocus.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/CBaseCharacter.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Utilities/CheckMacros.h"

UCBTTask_SetFocus::UCBTTask_SetFocus()
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UCBTTask_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	
	// Focus 해제
	if (bFocus == false)
	{
		if (AIController != nullptr)
			AIController->SetFacialFocus(nullptr);
	
		AIController->SetFocus(nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FocusActorKey.SelectedKeyName, nullptr);
		return EBTNodeResult::Succeeded;
	}

	// Focus 설정
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));

	if(Target != nullptr)
	{
		if (bFacialFocus)
		{
			ACBaseCharacter* BCTarget = Cast<ACBaseCharacter>(Target);
			if (AIController != nullptr && BCTarget != nullptr)
			{
				AIController->SetFacialFocus(BCTarget);
			}
		}

		AIController->SetFocus(Target);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FocusActorKey.SelectedKeyName, Target);

		return EBTNodeResult::Succeeded;
	}

	// 만약, Key가 Vector라면
	TSubclassOf<UBlackboardKeyType> KeyType = BlackboardKey.SelectedKeyType;
	if (KeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		AIController->SetFocalPoint(OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey()));
		ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(AIController->GetPawn());
		if (Enemy != nullptr)
		{
			Enemy->SetActorRotation((AIController->GetFocalPoint() - Enemy->GetActorLocation()).Rotation());
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
