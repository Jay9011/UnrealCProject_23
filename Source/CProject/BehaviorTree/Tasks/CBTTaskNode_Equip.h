#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CWeaponComponent.h"
#include "CBTTaskNode_Equip.generated.h"

/**
 *	@brief 장비 장착 Task
 */
UCLASS()
class CPROJECT_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_Equip();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EEquipSlotType EquipSlotType = EEquipSlotType::Max;
};
