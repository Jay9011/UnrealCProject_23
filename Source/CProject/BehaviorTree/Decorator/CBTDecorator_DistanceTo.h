#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "CBTDecorator_DistanceTo.generated.h"

/**
 * @brief Blackboard의 Vector나 Object 타입의 키와 현재 Actor의 위치를 비교하여 거리를 계산하는 Decorator
 */
UCLASS()
class CPROJECT_API UCBTDecorator_DistanceTo : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:
	UCBTDecorator_DistanceTo();

protected:
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CompareDistance(float Distance) const;
	bool CompareDistance(const FVector& InLocation, const FVector& InTargetLocation) const;
};
