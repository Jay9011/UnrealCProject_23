#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_Target.generated.h"

/**
 *	@brief EQS Target Context
 */
UCLASS()
class CPROJECT_API UCEnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()
private:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};