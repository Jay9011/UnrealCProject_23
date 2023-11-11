#pragma once

#include "CoreMinimal.h"
#include "Character/CEnemy.h"
#include "CEnemy_HumanType.generated.h"

/**
 * @brief 인간형 Enemy
 */
UCLASS()
class CPROJECT_API ACEnemy_HumanType : public ACEnemy
{
	GENERATED_BODY()

public:
	ACEnemy_HumanType();

protected:
	virtual void BeginPlay() override;
	
/*
 *	기본 컴포넌트 
 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	class UCMovementComponent* Movement;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UCWeaponComponent* Weapon;
};
