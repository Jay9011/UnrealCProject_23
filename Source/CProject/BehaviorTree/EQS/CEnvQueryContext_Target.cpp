#include "BehaviorTree/EQS/CEnvQueryContext_Target.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Utilities/CheckMacros.h"

void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* QueryOwner = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	ACAIController* Controller = Cast<ACAIController>(QueryOwner->GetController());
	CheckNull(Controller);

	UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
	CheckNull(Blackboard);
	
	UCAIBehaviorComponent* Behavior = Controller->GetBehaviorComponent();
	CheckNull(Behavior);

	AActor* Actor = Cast<AActor>(Blackboard->GetValueAsObject(Behavior->GetTargetKey()));
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Actor);
}
