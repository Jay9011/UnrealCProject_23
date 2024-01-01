#pragma once

#include "CoreMinimal.h"
#include "Weapons/CtrlActions/CGuardAction.h"
#include "CGuardAction_Blade.generated.h"

class UTimelineComponent;
class UCParryAction;
/**
 * @brief Blade의 가드 액션
 */
UCLASS(Blueprintable)
class CPROJECT_API UCGuardAction_Blade : public UCGuardAction
{
	GENERATED_BODY()
public:
	UCGuardAction_Blade();
	
public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment) override;
	
public:
	virtual void BlockingSuccess(FDamagedData& DamagedData) override;
	virtual void GuardBreak(FDamagedData& DamagedData) override;
	virtual void ParryingSuccess(FDamagedData& DamagedData) override;

public:
	void AttackerProcess(ACharacter* InAttacker);
	void ParryingProcess(ACharacter* InAttacker);

	void ParryingDilationTimelineCallback(float Value);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UFXSystemAsset* MetalEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	FVector EffectScale = FVector(1.0f);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Parrying Action")
	TSubclassOf<UCParryAction> ParryActionClass;
	UPROPERTY()
	UCParryAction* ParryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Parrying Action")
	float ParryingDilation = 0.25f;
	UPROPERTY(EditDefaultsOnly, Category = "Parrying Action")
	UCurveFloat* ParryingDilationCurve;
	UPROPERTY(EditDefaultsOnly, Category = "Parrying Action")
	UAnimMontage* ParriedMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Parrying Action")
	float PlayRate = 1.0f;
};
