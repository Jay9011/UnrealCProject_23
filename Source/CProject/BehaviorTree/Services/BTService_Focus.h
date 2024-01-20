#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Focus.generated.h"

/**
 *	@brief Focus 관리 Service
 */
UCLASS()
class CPROJECT_API UBTService_Focus : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_Focus();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Focus")
	FBlackboardKeySelector FocusActorKey;
};
