#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

/**
 * @brief 순찰
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Patrol();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void DrawDebug(const UWorld* InWorld, const FVector& InLocation);

	void InterpRotation(UBehaviorTreeComponent& OwnerComp, float DeltaSeconds);
	
private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bDebugMode = false;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptanceDistance = 20.f;

	UPROPERTY(EditAnywhere, Category = "Patrol|Option")
	uint8 MaxAttempts = 10;
};
