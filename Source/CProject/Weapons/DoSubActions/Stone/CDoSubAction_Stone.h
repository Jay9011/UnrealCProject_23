#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoSubAction.h"
#include "CDoSubAction_Stone.generated.h"

class UCDoAction_Stone;
class UCPredictionPathComponent;
/**
 *	@brief Stone SubAction 클래스
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_Stone : public UCDoSubAction
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

	virtual void Tick_Implementation(float InDeltaTime) override;
	
public:
	virtual void Pressed() override;
	virtual void Released() override;

private:
	TWeakObjectPtr<UCPredictionPathComponent> PredictionPath = nullptr;
	TWeakObjectPtr<UCDoAction_Stone> StoneAction = nullptr;
};
