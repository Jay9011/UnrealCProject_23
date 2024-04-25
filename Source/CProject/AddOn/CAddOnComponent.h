#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAddOnComponent.generated.h"

class UCWeaponObject;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCAddOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon);

protected:
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

	UPROPERTY()
	UCWeaponObject* Weapon = nullptr;
};
