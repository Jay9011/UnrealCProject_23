#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_StandingType.generated.h"

/**
 *	@brief StandingType을 변경하는 Service
 */
UCLASS()
class CPROJECT_API UCBTService_StandingType : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_StandingType();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName StandingTypeKey = "StandingType";
};
