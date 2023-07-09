#include "Components/IEvadeAction.h"

#include "GameFramework/Character.h"

void FEvadeData::PlayMontage(ACharacter* InOwner)
{
	if(!!Montage)
	{
		InOwner->PlayAnimMontage(Montage, PlayRate);
	}
}
