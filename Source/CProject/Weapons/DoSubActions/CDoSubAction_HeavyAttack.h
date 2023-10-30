#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoSubAction_HeavyAttack.generated.h"

/**
 * @brief 강공격 SubAction
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_HeavyAttack : public UCDoSubAction
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;
	
public:
	virtual void Pressed() override;

	virtual void BeginSubAction_Implementation() override;
	virtual void EndSubAction_Implementation() override;

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;

private:
	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<FDoActionData> HeavyAttackDatas;
	
private:
	// 현재 데미지를 입은 캐릭터들
	TArray<class ACharacter*> DamagedCharacters;
	int32 HitIndex;

	// CDoAction_Combo 인 경우 해당 index를 사용해야 하므로 다운캐스팅 해서 사용한다.
	UPROPERTY()
	class UCDoAction_Combo* DoAction_Combo;

	// HeavyAttack의 Combo 시스템은 DoActionCombo에서 가져온다.
	UPROPERTY()
	class UCComboState* ComboState;
};
