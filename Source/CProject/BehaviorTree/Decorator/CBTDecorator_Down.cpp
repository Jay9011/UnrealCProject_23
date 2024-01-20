#include "BehaviorTree/Decorator/CBTDecorator_Down.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"

UCBTDecorator_Down::UCBTDecorator_Down()
{
	NodeName = "Down";
}

bool UCBTDecorator_Down::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());

	UCMovementComponent* Movement = Cast<UCMovementComponent>(Enemy_AI->GetComponentByClass(UCMovementComponent::StaticClass()));
	
	return Movement->IsDown();
}
