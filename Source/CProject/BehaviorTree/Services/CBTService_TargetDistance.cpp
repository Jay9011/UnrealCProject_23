#include "BehaviorTree/Services/CBTService_TargetDistance.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Utilities/CheckMacros.h"

UCBTService_TargetDistance::UCBTService_TargetDistance()
{
	NodeName = "Target Distance";

	bNotifyTick = true;
	Interval = 0.5f;
	RandomDeviation = 0.f;
}

void UCBTService_TargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* Controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNull(Controller);

	ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(Controller->GetPawn());
	CheckNull(Enemy);

	FVector Location = Enemy->GetActorLocation();
	
	TSubclassOf<UBlackboardKeyType> KeyType = BlackboardKey.SelectedKeyType;

	if (KeyType)
	{
		FVector TargetLocation;
		
		if (KeyType == UBlackboardKeyType_Vector::StaticClass())
		{
			TargetLocation = Controller->GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);
		}
		else if (KeyType == UBlackboardKeyType_Object::StaticClass())
		{
			UObject* TargetObject = Controller->GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName);
			CheckNull(TargetObject);

			AActor* TargetActor = Cast<AActor>(TargetObject);
			CheckNull(TargetActor);

			TargetLocation = TargetActor->GetActorLocation();
		}

		float Distance = FVector::Distance(Location, TargetLocation);
		Controller->GetBlackboardComponent()->SetValueAsFloat(SaveDistanceKey.SelectedKeyName, Distance);
	}
}
