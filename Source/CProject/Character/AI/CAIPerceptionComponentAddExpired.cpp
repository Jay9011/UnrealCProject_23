#include "Character/AI/CAIPerceptionComponentAddExpired.h"

void UCAIPerceptionComponentAddExpired::HandleExpiredStimulus(FAIStimulus& StimulusStore)
{
	if (OnHandleExpired.IsBound())
	{
		OnHandleExpired.Broadcast(StimulusStore);
	}
}
