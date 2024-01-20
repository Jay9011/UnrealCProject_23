#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_WeaponComponent.generated.h"

class UCWeaponComponent;

UENUM()
enum class EWeaponComponentDeco : uint8
{
	IsEquipped,
};

/**
 *	@brief WeaponComponent와 관련된 Decorator
 */
UCLASS()
class CPROJECT_API UCBTDecorator_WeaponComponent : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCBTDecorator_WeaponComponent();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CheckWeaponEquipped(UCWeaponComponent* WeaponComponent) const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	EWeaponComponentDeco WeaponComponentDeco = EWeaponComponentDeco::IsEquipped;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bCheck = false;
};
