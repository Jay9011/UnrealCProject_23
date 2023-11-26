#pragma once

#include "CoreMinimal.h"
#include "Weapons/CComboState.h"
#include "Weapons/CDoAction.h"
#include "Weapons/IComboState.h"
#include "MyDebugger/IDebugCollector.h"
#include "CDoAction_Combo.generated.h"

/**
 * @brief 콤보 동작 관련 클래스
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoAction_Combo : public UCDoAction
	, public IIComboState
	, public IIDebugCollector
{
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner) override;
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, UCComboState* InComboState);
	
public:
	// virtual void DoAction() override;
	virtual void DoAction_Implementation() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Add Event", meta = (DisplayName = "Add DoAction_Combo", ToolTip = "이 DoAction_Combo에서 사용할 DoAction_Combo를 추가합니다."))
	UCDoAction_Combo* BP_AddDoAction_Combo(TSubclassOf<UCDoAction_Combo> InDoActionClass, UCComboState* InComboState);
	
/*
 * 충돌 관련 바운딩 함수
 */
public:
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther) override;


/*
 * Getter / Setter
 */
public:
	UFUNCTION(BlueprintCallable, BlueprintGetter, Category = "Combo")
	virtual UCComboState* GetComboState() override { return ComboState; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	UCComboState* ComboState;

// IIDebugCollector
#if WITH_EDITOR
  public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
