#pragma once

#include "CoreMinimal.h"
#include "Components/CMovementComponent.h"
#include "CMovementComponent_AI.generated.h"

class ACAIController;
/**
 *	@brief AI용 Movement Component 
 */
UCLASS(ClassGroup=("Movement"), meta=(BlueprintSpawnableComponent))
class CPROJECT_API UCMovementComponent_AI : public UCMovementComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Move() override;
	virtual void Stop() override;

private:
	TWeakObjectPtr<ACAIController> OwnerAIController = nullptr;
};
