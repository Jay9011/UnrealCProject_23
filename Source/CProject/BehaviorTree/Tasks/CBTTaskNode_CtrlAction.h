#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_CtrlAction.generated.h"

/**
 *	@brief Ctrl Action을 수행하는 Task
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_CtrlAction : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_CtrlAction();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	bool bDoPressCtrl = true;
};
