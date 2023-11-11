#pragma once

#include "CoreMinimal.h"
#include "Character/CBaseCharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

/**
 *	@brief Enemy Base
 */
UCLASS()
class CPROJECT_API ACEnemy : public ACBaseCharacter
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;
};
