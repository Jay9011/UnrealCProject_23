#pragma once

#include "CoreMinimal.h"
#include "Weapons/CComboState.h"
#include "Weapons/CDoAction.h"
#include "Weapons/IComboState.h"
#include "CDoAction_Combo.generated.h"

/**
 * @brief 콤보 동작 관련 클래스
 */
UCLASS()
class CPROJECT_API UCDoAction_Combo : public UCDoAction
	, public IIComboState
{
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas) override;
	
public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;

public:
	virtual UCComboState* GetComboState() override { return ComboState; }
	
private:
	UPROPERTY()
	UCComboState* ComboState;

// IIDoActionDebugData
#if WITH_EDITOR
  public:
	virtual TArray<FString> GetDebugInfo() override;
#endif
};
