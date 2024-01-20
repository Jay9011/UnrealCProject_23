#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Guard.generated.h"

/**
 *	@brief 가드 액션을 수행하는 Task
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Guard : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Guard();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	bool bDoGuard = true;
};
