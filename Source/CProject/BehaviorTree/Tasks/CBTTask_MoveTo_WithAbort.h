#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTTask_MoveTo_WithAbort.generated.h"

/**
 * 
 */
UCLASS()
class CPROJECT_API UCBTTask_MoveTo_WithAbort : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
