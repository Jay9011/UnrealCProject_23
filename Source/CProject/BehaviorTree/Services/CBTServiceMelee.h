#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTServiceMelee.generated.h"

/**
 * @brief Melee 테스트용
 */
UCLASS()
class CPROJECT_API UCBTServiceMelee : public UBTService
{
	GENERATED_BODY()
public:
	UCBTServiceMelee();
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float MeleeRange = 150.f;
	UPROPERTY(EditAnywhere, Category = "Action")
	float MaxMeleeRange = 200.f;
};
