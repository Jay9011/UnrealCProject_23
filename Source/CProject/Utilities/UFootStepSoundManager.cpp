#include "Utilities/UFootStepSoundManager.h"

#include "CheckMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

bool UFootStepSoundManager::PlayFootStepSound(AActor* InInstigator, const FVector& InLocation, const FFootStepRowData& InTableRow, const FFootStepSoundData& InData)
{
	CheckNullResult(InInstigator, false);
	CheckNullResult(InTableRow.Sound, false);
	
	UGameplayStatics::PlaySoundAtLocation
	(
		InInstigator
		, InTableRow.Sound
		, InLocation
		, FMath::Clamp<float>(InData.Loudness * InTableRow.VolumeMultiplier, 0.1f, 1.f)
	);

	UAISense_Hearing::ReportNoiseEvent(InInstigator, InLocation, InData.Loudness, InInstigator, InData.MaxRange);

	return true;
}

bool UFootStepSoundManager::PlayAndReportNoiseEvent(AActor* InInstigator, const FVector& InLocation, USoundBase* InSound, float InLoudness, float InMaxRange)
{
	CheckNullResult(InInstigator, false);
	CheckNullResult(InSound, false);
	
	UGameplayStatics::PlaySoundAtLocation
	(
		InInstigator
		, InSound
		, InLocation
		, FMath::Clamp<float>(InLoudness, 0.1f, 1.f)
	);

	UAISense_Hearing::ReportNoiseEvent(InInstigator, InLocation, InLoudness, InInstigator, InMaxRange);

	return true;
}
