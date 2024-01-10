#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Character/DataAssets/CStandUpAsset.h"
#include "BTTask_BBStandUp.generated.h"

/**
 * @brief AI가 일어나는 행동을 수행하는 Task
 */
UCLASS()
class CPROJECT_API UBTTask_BBStandUp : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_BBStandUp();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "StandUp")
	bool UseDir = false;
	UPROPERTY(EditAnywhere, Category = "StandUp|Target", meta = (EditCondition = "!UseDir"))
	bool TowardTarget = false;
	UPROPERTY(EditAnywhere, Category = "StandUp|Dir", meta = (EditCondition = "UseDir"))
	EDir Dir = EDir::InPlace;
};
