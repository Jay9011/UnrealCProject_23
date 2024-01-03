#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Hitted.generated.h"

/**
 *	@brief 타격 받을시 실행할 Task
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Hitted : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Hitted();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
