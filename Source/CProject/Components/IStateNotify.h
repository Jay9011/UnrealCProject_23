#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IStateNotify.generated.h"

UINTERFACE(MinimalAPI)
class UIStateNotify : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief State Notify 관련 인터페이스
 */
class CPROJECT_API IIStateNotify
{
	GENERATED_BODY()

public:
	virtual void End_Evade(){}
};
