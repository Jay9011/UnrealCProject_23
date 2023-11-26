#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IExcuteAction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIExcuteAction : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief 동작을 실행하는 인터페이스
 */
class CPROJECT_API IIExcuteAction
{
	GENERATED_BODY()
	
public:
	virtual void Begin_Action() = 0;
	virtual void End_Action() = 0;

	virtual void ChangingProcess() {};

	virtual FString GetActionName() { return TEXT("IIExcuteAction"); }
};
