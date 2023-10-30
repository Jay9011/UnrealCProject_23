#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponAssetDebugInfo.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIWeaponAssetDebugInfo : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief WeaponAsset 관련 디버그 데이터 인터페이스
 */
class MYDEBUGGER_API IIWeaponAssetDebugInfo
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
  virtual TArray<FString> GetDebugInfo() { return {}; }
#endif
};
