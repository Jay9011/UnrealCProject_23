#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CMovementComponent.h"
#include "CBTTaskNode_Speed.generated.h"

/**
 * @brief 속도 조절
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Speed();
	
public:
	// Task가 실행될 때 (starts this task, should return Succeeded, Failed or InProgress)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// Task가 취소될 때 (Aborted or InProgress)
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Task가 완료될 때
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "Speed|Type")
	ESpeedType SpeedType = ESpeedType::Walk;
};
