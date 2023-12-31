#pragma once

#include "CoreMinimal.h"
#include "Weapons/CGuardMontageAsset.h"
#include "CGuardBeginEndMontageAsset.generated.h"

/**
 *	@brief 가드 시작에 루프가 포함된 몽타주 에셋
 */
UCLASS(Blueprintable)
class CPROJECT_API UCGuardBeginEndMontageAsset : public UCGuardMontageAsset
{
	GENERATED_BODY()
public:
	virtual void PlayStartBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) override;
	virtual void PlayLoopBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) override;
	virtual void PlaySuccessBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) override;
	virtual void PlayBreakBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) override;
	virtual void PlayEndBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Guard")
	FGuardMontageData GuardMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Guard Success")
	FGuardMontageData GuardSuccessMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Guard Break")
	FGuardMontageData GuardBreakMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Guard End")
	FGuardMontageData GuardEndMontage;
};
