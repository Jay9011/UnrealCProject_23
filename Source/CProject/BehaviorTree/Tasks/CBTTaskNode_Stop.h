#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Stop.generated.h"

/**
 * @brief Stop Movement Task 
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Stop : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Stop();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
