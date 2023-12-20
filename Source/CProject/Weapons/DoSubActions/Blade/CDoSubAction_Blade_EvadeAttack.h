#pragma once

#include "CoreMinimal.h"
#include "Interface/FHitResultInterface.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoSubAction_Blade_EvadeAttack.generated.h"

/**
 *	@brief 저스트 회피 후 공격
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_Blade_EvadeAttack : public UCDoSubAction
{
	GENERATED_BODY()

public:
	UCDoSubAction_Blade_EvadeAttack();

protected:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	virtual void Pressed() override;
	virtual void Released() override;

public:
	virtual void Begin_Action() override;
	virtual void End_Action() override;

/*
 * 공격형 액션 관리
 */
public:
	UPROPERTY(EditAnywhere, Category = "Action")
	FActionData ActionData;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<class ACSkill> SkillClass;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	FVector OffsetLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float SkillDelay = 0.25f;
	
	FTimerHandle SkillDelayTimerHandle;
};
