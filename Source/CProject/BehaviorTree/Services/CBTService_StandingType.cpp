#include "BehaviorTree/Services/CBTService_StandingType.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CMovementComponent.h"
#include "Utilities/CheckMacros.h"

UCBTService_StandingType::UCBTService_StandingType()
{
	NodeName = "StandingType Check";

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UCBTService_StandingType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(OwnerController);

	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	CheckNull(Enemy_AI);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	CheckNull(Blackboard);

	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Enemy_AI->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(MovementComponent);

	const EStandingType BlackboardStandingType = static_cast<EStandingType>(Blackboard->GetValueAsEnum(StandingTypeKey));
	const EStandingType MovementStandingType = MovementComponent->GetStandingType();
	
	if (MovementStandingType != BlackboardStandingType)
	{
		Blackboard->SetValueAsEnum(StandingTypeKey, static_cast<uint8>(MovementStandingType));
	}
}
