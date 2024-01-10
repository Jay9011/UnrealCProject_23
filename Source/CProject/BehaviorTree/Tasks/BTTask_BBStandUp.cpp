#include "BehaviorTree/Tasks/BTTask_BBStandUp.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CBaseCharacter.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"

UBTTask_BBStandUp::UBTTask_BBStandUp()
{
	NodeName = "StandUp";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_BBStandUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Character = Cast<ACEnemy_AI>(AIController->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Character->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (MovementComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 방향을 사용하는 경우
	if (UseDir)
	{
		MovementComponent->StandUp(Dir);
		return EBTNodeResult::InProgress;
	}

	// 타겟을 기준으로 사용하는 경우
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (Target == nullptr)
	{
		// 타켓이 없는 경우, 제자리에서 일어난다.
		MovementComponent->StandUp(EDir::InPlace);
		return EBTNodeResult::InProgress;
	}

	FVector CharacterForward = Character->GetActorForwardVector();
	FVector ToTarget = (Target->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();

	float Dot = FVector::DotProduct(CharacterForward, ToTarget);

	if (Dot > 0.5f)
	{
		if (TowardTarget)
		{
			MovementComponent->StandUp(EDir::FRONT);			
		}
		else
		{
			MovementComponent->StandUp(EDir::BACK);
		}
	}
	else if (Dot < -0.5f)
	{
		if (TowardTarget)
		{
			MovementComponent->StandUp(EDir::BACK);
		}
		else
		{
			MovementComponent->StandUp(EDir::FRONT);
		}
	}
	else
	{
		// 오른쪽 왼쪽 방향 결정
		FVector Cross = FVector::CrossProduct(CharacterForward, ToTarget);
		if (Cross.Z > 0.0f)
		{
			if (TowardTarget)
			{
				MovementComponent->StandUp(EDir::LEFT);				
			}
			else
			{
				MovementComponent->StandUp(EDir::RIGHT);
			}
		}
		else
		{
			if (TowardTarget)
			{
				MovementComponent->StandUp(EDir::RIGHT);
			}
			else
			{
				MovementComponent->StandUp(EDir::LEFT);
			}
		}
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_BBStandUp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Character = Cast<ACEnemy_AI>(AIController->GetPawn());
	if (Character == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Character->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (MovementComponent == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
		return;
	}

	// 기상이 끝났는지 확인
	if (MovementComponent->bStandingProcessIsDone())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
}
