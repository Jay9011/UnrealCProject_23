#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

/**
 *  @brief Test Action Task
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Action();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Combo")
	bool bUseCombo = true;
	UPROPERTY(EditAnywhere, Category = "Combo")
	bool bUseRandomCombo = true;

	int32 RandomComboIndex = 0; 
};
