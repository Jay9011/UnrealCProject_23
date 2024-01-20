#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CAIPerceptionComponentAddExpired.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHandleExpiredDelegate, FAIStimulus&, StimulusStore);

/**
 *	@brief  Expired를 추가한 AIPerceptionComponent
 */
UCLASS()
class CPROJECT_API UCAIPerceptionComponentAddExpired : public UAIPerceptionComponent
{
	GENERATED_BODY()

protected:
	virtual void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;

public:
	FHandleExpiredDelegate OnHandleExpired;
};
