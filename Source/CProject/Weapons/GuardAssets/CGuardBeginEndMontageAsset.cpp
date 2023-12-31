#include "Weapons/GuardAssets/CGuardBeginEndMontageAsset.h"

#include "Utilities/CheckMacros.h"

void UCGuardBeginEndMontageAsset::PlayStartBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData)
{
	CheckNull(InAnimInstance);
	CheckNull(GuardMontage.Montage);
	
	InAnimInstance->Montage_Play(GuardMontage.Montage, GuardMontage.PlayRate);
	InGuardMontageData = GuardMontage;
}

void UCGuardBeginEndMontageAsset::PlayLoopBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData)
{
	CheckNull(InAnimInstance);
	CheckNull(GuardMontage.Montage);
	
	InAnimInstance->Montage_Play(GuardMontage.Montage, GuardMontage.PlayRate);
	InAnimInstance->Montage_JumpToSection(LoopSectionName, GuardMontage.Montage);

	InGuardMontageData = GuardMontage;
}

void UCGuardBeginEndMontageAsset::PlaySuccessBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData)
{
	CheckNull(InAnimInstance);
	CheckNull(GuardSuccessMontage.Montage);
	
	InAnimInstance->Montage_Play(GuardSuccessMontage.Montage, GuardSuccessMontage.PlayRate);
	
	InGuardMontageData = GuardMontage;
}

void UCGuardBeginEndMontageAsset::PlayBreakBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData)
{
	CheckNull(InAnimInstance);
	CheckNull(GuardBreakMontage.Montage);
	
	InAnimInstance->Montage_Play(GuardBreakMontage.Montage, GuardBreakMontage.PlayRate);
	
	InGuardMontageData = GuardMontage;
}

void UCGuardBeginEndMontageAsset::PlayEndBlockMontage(UAnimInstance* InAnimInstance, FGuardMontageData& InGuardMontageData)
{
	CheckNull(InAnimInstance);
	CheckNull(GuardEndMontage.Montage);
	
	InAnimInstance->Montage_Play(GuardEndMontage.Montage, GuardEndMontage.PlayRate);
	
	InGuardMontageData = GuardMontage;
}
