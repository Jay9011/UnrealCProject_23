#include "Weapons/DoSubActions/Blade/CDoSubAction_Blade_EvadeAttack.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/CSkill.h"
#include "Weapons/CWeaponObject.h"

UCDoSubAction_Blade_EvadeAttack::UCDoSubAction_Blade_EvadeAttack()
{
}

void UCDoSubAction_Blade_EvadeAttack::BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon,
                                                ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InWeapon, InAttachment, InDoAction);

	CheckNull(InOwner);
	
	ActionData.InitActionData();
}

void UCDoSubAction_Blade_EvadeAttack::Pressed()
{
	Super::Pressed();

	StateComponent->SetActionMode();
	StateComponent->OnSubActionMode();
	
	Weapon->SetCurrentAction(this);
	if(ActionData.ActionData.Num() > 0)
		ActionData.ActionData[0].DoAction(OwnerCharacter);

	// 스킬 소환
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwnerCharacter;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
		FTransform SpawnTransform = OwnerCharacter->GetActorTransform();
		SpawnTransform.AddToTranslation(OffsetLocation);
			
		ACSkill* Skill = OwnerCharacter->GetWorld()->SpawnActor<ACSkill>(SkillClass, SpawnTransform, SpawnParams);
		
		FTimerDelegate SkillDelayTimerDelegate;
		SkillDelayTimerDelegate.BindLambda([this, Skill]()
		{
			Skill->ActiveSkill();
			
		});
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillDelayTimerHandle, SkillDelayTimerDelegate, SkillDelay, false);
	}
	
}

void UCDoSubAction_Blade_EvadeAttack::Released()
{
	Super::Released();
}

void UCDoSubAction_Blade_EvadeAttack::Begin_Action()
{
	Super::Begin_Action();
}

void UCDoSubAction_Blade_EvadeAttack::End_Action()
{
	Super::End_Action();

	StateComponent->SetIdleMode();
	StateComponent->OffSubActionMode();

	MovementComponent->Move();
	MovementComponent->DisableFixedCamera();
	
	Weapon->UpdateActions();
}