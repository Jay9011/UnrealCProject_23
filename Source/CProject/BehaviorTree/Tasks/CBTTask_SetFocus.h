#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTask_SetFocus.generated.h"

/**
 * @brief Focus 설정
 */
UCLASS()
class CPROJECT_API UCBTTask_SetFocus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UCBTTask_SetFocus();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Focus")
	bool bFocus = true;
	UPROPERTY(EditAnywhere, Category = "Focus")
	bool bFacialFocus = false;
	UPROPERTY(EditAnywhere, Category = "Focus")
	FBlackboardKeySelector FocusActorKey;
};
