#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Utilities/UFootStepSoundManager.h"
#include "CAnimNotify_MakeNoise.generated.h"

/**
 * @brief 발소리를 내고 AI에게 알리는 AnimNotify.
 */
UCLASS()
class CPROJECT_API UCAnimNotify_MakeNoise : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere)
	FFootStepSoundData SoundData;
};
