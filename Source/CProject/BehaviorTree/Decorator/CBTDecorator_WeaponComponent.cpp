#include "BehaviorTree/Decorator/CBTDecorator_WeaponComponent.h"

#include "Character/AI/CAIController.h"
#include "Character/AI/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Utilities/CheckMacros.h"

UCBTDecorator_WeaponComponent::UCBTDecorator_WeaponComponent()
{
	NodeName = "WeaponComponent";
}

bool UCBTDecorator_WeaponComponent::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACAIController* OwnerController = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* Enemy_AI = Cast<ACEnemy_AI>(OwnerController->GetPawn());
	CheckNullResult(Enemy_AI, false);

	UCWeaponComponent* WeaponComponent = Cast<UCWeaponComponent>(Enemy_AI->GetComponentByClass(UCWeaponComponent::StaticClass()));
	CheckNullResult(WeaponComponent, false);

	switch (WeaponComponentDeco) {
	case EWeaponComponentDeco::IsEquipped:
		{
			return CheckWeaponEquipped(WeaponComponent);
		}
		break;
	}

	return false;
}

bool UCBTDecorator_WeaponComponent::CheckWeaponEquipped(UCWeaponComponent* WeaponComponent) const
{
	return !(WeaponComponent->IsUnarmedMode() ^ !bCheck);
}
