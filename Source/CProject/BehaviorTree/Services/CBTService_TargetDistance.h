#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "CBTService_TargetDistance.generated.h"

/**
 * @brief Target과의 거리를 계산하여 Blackboard에 저장하는 Service
 */
UCLASS()
class CPROJECT_API UCBTService_TargetDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UCBTService_TargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector SaveDistanceKey;
};
