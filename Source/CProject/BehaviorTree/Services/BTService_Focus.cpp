#include "BehaviorTree/Services/BTService_Focus.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Character/AI/CAIController.h"

UBTService_Focus::UBTService_Focus()
{
	NodeName = "Focus";

	FocusActorKey.SelectedKeyName = "Focus";
	FocusActorKey.SelectedKeyType = UBlackboardKeyType_Object::StaticClass();

	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UBTService_Focus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());

	OwnerController->SetFocus(Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FocusActorKey.SelectedKeyName)));
}
