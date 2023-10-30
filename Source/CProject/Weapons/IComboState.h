#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IComboState.generated.h"

UINTERFACE(MinimalAPI)
class UIComboState : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief 콤보 상태를 나타내는 인터페이스
 */
class CPROJECT_API IIComboState
{
	GENERATED_BODY()

public:
	virtual class UCComboState* GetComboState() = 0;
};
