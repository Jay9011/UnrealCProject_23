#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTask_SearchMontage.generated.h"

/**
 *	@brief 특정 위치로 가서 탐색 애니메이션을 재생합니다.
 *	@ref CAIPatrolComponent::PlaySearchMontage
 *	@ref CMovementComponent
 */
UCLASS()
class CPROJECT_API UCBTTask_SearchMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTask_SearchMontage();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
