#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Components/CGuardComponent.h"
#include "CBTDecorator_GuardComponent.generated.h"

/**
 *	@brief GuardComponent와 관련된 Decorator
 */
UCLASS()
class CPROJECT_API UCBTDecorator_GuardComponent : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCBTDecorator_GuardComponent();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Guard")
	EGuardType GuardType = EGuardType::None;
};
