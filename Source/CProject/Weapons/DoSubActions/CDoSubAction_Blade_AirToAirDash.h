#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoSubAction.h"
#include "CDoSubAction_Blade_AirToAirDash.generated.h"

/**
 * 
 */
UCLASS()
class CPROJECT_API UCDoSubAction_Blade_AirToAirDash : public UCDoSubAction
{
	GENERATED_BODY()

private:
	virtual void BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	virtual void Pressed() override;
};
