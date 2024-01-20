#include "BehaviorTree/Decorator/CBTDecorator_GuardComponent.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"

UCBTDecorator_GuardComponent::UCBTDecorator_GuardComponent()
{
	NodeName = "GuardComponent";
}

bool UCBTDecorator_GuardComponent::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	if (Enemy_AI == nullptr)
	{
		return false;
	}

	UCGuardComponent* GuardComponent = Cast<UCGuardComponent>(Enemy_AI->GetComponentByClass(UCGuardComponent::StaticClass()));
	if (GuardComponent == nullptr)
	{
		return false;
	}

	switch (GuardType) {
	case EGuardType::None:
		return GuardComponent->IsUnGuard();
	case EGuardType::Blocking:
		return GuardComponent->IsBlocking();
	case EGuardType::Parrying:
		return GuardComponent->IsParrying();
	}
	
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
