#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Chaos/ChaosEngineInterface.h"
#include "UFootStepSoundManager.generated.h"

USTRUCT(BlueprintType)
struct FFootStepSoundData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float Loudness = 0.5f;
	
	UPROPERTY(EditAnywhere)
	float MaxRange = 0.f;
};

USTRUCT()
struct FFootStepRowData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	
	UPROPERTY(EditAnywhere)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere)
	float VolumeMultiplier = 0.5f;
};

/**
 *	@brief 발소리를 관리하는 매니저 클래스.
 */
UCLASS()
class CPROJECT_API UFootStepSoundManager: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void PlayFootStepSound(AActor* InInstigator, const FVector& InLocation, const FFootStepRowData& InTableRow, const FFootStepSoundData& InData);
};
