#include "BehaviorTree/Services/CBTService_MovementComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Utilities/CheckMacros.h"

UCBTService_MovementComponent::UCBTService_MovementComponent()
{
	NodeName = "Movement";

	CanMoveKey.SelectedKeyName = "CanMove";
	CanMoveKey.SelectedKeyType = UBlackboardKeyType_Bool::StaticClass();

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UCBTService_MovementComponent::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	CheckNull(Enemy_AI);
	
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Enemy_AI->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(MovementComponent);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(CanMoveKey.SelectedKeyName, MovementComponent->CanMove());
}
