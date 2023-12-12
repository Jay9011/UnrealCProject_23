#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FHitResultInterface.generated.h"

UINTERFACE(MinimalAPI)
class UFHitResultInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * FHitResult와 관련된 인터페이스
 */
class CPROJECT_API IFHitResultInterface
{
	GENERATED_BODY()

public:
	virtual void SetFHitResult(const FHitResult& InHitResult) = 0;
	virtual const FHitResult& GetFHitResult() const = 0;
};
