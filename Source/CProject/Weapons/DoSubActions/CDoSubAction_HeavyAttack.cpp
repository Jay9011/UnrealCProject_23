#include "Weapons/DoSubActions/CDoSubAction_HeavyAttack.h"

#include "Components/CStateComponent.h"
#include "Utilities/CheckMacros.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

void UCDoSubAction_HeavyAttack::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	DoAction_Combo = Cast<UCDoAction_Combo>(InDoAction);
}

void UCDoSubAction_HeavyAttack::Pressed()
{
	CheckTrue(StateComponent->IsSubActionMode());
	CheckFalse(HeavyAttackDatas.IsValidIndex(DoAction_Combo->GetIndex()));
	
	Super::Pressed();

	StateComponent->SetActionMode();
	StateComponent->OnSubActionMode();

	
}

void UCDoSubAction_HeavyAttack::BeginSubAction_Implementation()
{
	Super::BeginSubAction_Implementation();
}

void UCDoSubAction_HeavyAttack::EndSubAction_Implementation()
{
	Super::EndSubAction_Implementation();
}
