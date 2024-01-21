#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_LostTargetLocation.generated.h"

/**
 *	@brief LostTargetLocation을 기준으로 하는 Query Context
 */
UCLASS()
class CPROJECT_API UCEnvQueryContext_LostTargetLocation : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
