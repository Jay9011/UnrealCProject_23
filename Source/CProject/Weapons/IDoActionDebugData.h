#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDoActionDebugData.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDoActionDebugData : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief DoAction 관련 디버그 데이터 인터페이스
 */
class CPROJECT_API IIDoActionDebugData
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
  virtual TArray<FString> GetDebugInfo() const { return {}; }
#endif
};
