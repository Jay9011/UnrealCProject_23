#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTask_SetBlackboard_Int.generated.h"

/**
 *	@brief Int 값을 Blackboard에 저장하는 Task
 */
UCLASS()
class CPROJECT_API UCBTTask_SetBlackboard_Int : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UCBTTask_SetBlackboard_Int();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Key")
	int32 Value = 0;
};
