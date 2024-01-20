#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_Down.generated.h"

/**
 * @brief Down 상태 확인
 */
UCLASS()
class CPROJECT_API UCBTDecorator_Down : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCBTDecorator_Down();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
