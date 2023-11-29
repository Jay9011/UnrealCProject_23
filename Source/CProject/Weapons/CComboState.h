#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CComboState.generated.h"

/**
 * @brief  콤보 상태를 관리하는 클래스
 */
UCLASS(BlueprintType)
class CPROJECT_API UCComboState : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }
	FORCEINLINE void ReserveCombo() { bExist = true; }
	FORCEINLINE void ExpireCombo() { bExist = false; }
	FORCEINLINE int32 IncreaseCombo() { return ++Index; }
	FORCEINLINE void ResetCombo() { Index = 0; }

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combo")
	FORCEINLINE int32 GetIndex() const { return Index; }
	
	// 현재 콤보가 가능한 상태인가
	FORCEINLINE bool IsEnable() const { return bEnable; }
	// 현재 콤보가 예약되어있는 상태인가
	FORCEINLINE bool IsExist() const { return bExist; }
	
protected:
	int32 Index = 0;

	// 콤보가 활성화 되어있는지 여부
	bool bEnable = false;
	// 콤보 동작이 존재하는지 여부
	bool bExist = false;
};
