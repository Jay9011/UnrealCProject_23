#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_MovementComponent.generated.h"

/**
 * @brief MovementComponent를 사용하는 Actor의 Movement 관련 정보를 업데이트하는 Service
 */
UCLASS()
class CPROJECT_API UCBTService_MovementComponent : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_MovementComponent();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	FBlackboardKeySelector CanMoveKey;
};
