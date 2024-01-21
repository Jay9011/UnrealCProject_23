#include "BehaviorTree/EQS/CEnvQueryContext_LostTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"
#include "Utilities/CheckMacros.h"

void UCEnvQueryContext_LostTargetLocation::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	ACEnemy_AI* QueryOwner = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	CheckNull(QueryOwner);

	UCAIBehaviorComponent* BehaviorComponent = Cast<UCAIBehaviorComponent>(QueryOwner->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	CheckNull(BehaviorComponent);

	UBlackboardComponent* Blackboard = Cast<ACAIController>(QueryOwner->GetController())->GetBlackboardComponent();
	CheckNull(Blackboard);

	UEnvQueryItemType_Point::SetContextHelper(ContextData, Blackboard->GetValueAsVector(BehaviorComponent->GetLostTargetLocationKey()));
}
