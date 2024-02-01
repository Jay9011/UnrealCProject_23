#include "Utilities/UFootStepSoundManager.h"

#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"

void UFootStepSoundManager::PlayFootStepSound(AActor* InInstigator, const FVector& InLocation, const FFootStepRowData& InTableRow, const FFootStepSoundData& InData)
{
	UGameplayStatics::PlaySoundAtLocation
	(
		InInstigator
		, InTableRow.Sound
		, InLocation
		, FMath::Clamp<float>(InData.Loudness * InTableRow.VolumeMultiplier, 0.1f, 1.f)
	);

	UAISense_Hearing::ReportNoiseEvent(InInstigator, InLocation, InData.Loudness, InInstigator, InData.MaxRange);
}
