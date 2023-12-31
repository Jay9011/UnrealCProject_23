#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CParryAction.generated.h"

class ACBaseCharacter;
class ACharacter;

/**
 * @brief 패링 성공시 발동하는 액션
 */
UCLASS()
class CPROJECT_API UCParryAction : public UCDoAction
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay(class ACharacter* InOwner, class UCWeaponObject* InWeapon) override;
	
	virtual void End_DoAction() override;
	virtual void End_Action() override;

public:
	virtual void DoAction(ACharacter* InAttacker);

protected:
	UPROPERTY()
	ACBaseCharacter* OwnerBaseCharacter;
	ACharacter* Attacker;
};
