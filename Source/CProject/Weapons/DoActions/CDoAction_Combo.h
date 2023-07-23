#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/**
 * @brief 콤보 동작 관련 클래스
 */
UCLASS()
class CPROJECT_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	
private:
	int32 Index;

	bool bEnable;
	bool bExist;

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
};
