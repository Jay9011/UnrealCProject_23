#pragma once

#include "CoreMinimal.h"
#include "Weapons/CtrlActions/CGuardAction.h"
#include "CGuardAction_PirateSword.generated.h"

/**
 * @brief PirateSword의 가드 액션
 */
UCLASS(Blueprintable)
class CPROJECT_API UCGuardAction_PirateSword : public UCGuardAction
{
	GENERATED_BODY()
public:
	UCGuardAction_PirateSword();
	
public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment) override;

	virtual void BlockingSuccess(FDamagedData& DamagedData) override;
	virtual void GuardBreak(FDamagedData& DamagedData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UFXSystemAsset* MetalEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FVector EffectScale = FVector(1.0f);

};
