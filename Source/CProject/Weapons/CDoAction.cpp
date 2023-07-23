#include "Weapons/CDoAction.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(ACAttachment* InAttachment, UCEquipment* InEquipment, ACharacter* InOwner, const TArray<FDoActionData>& InDoActionDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld(); //UObject는 World가 없으므로, OwnerCharacter의 World를 가져온다.

	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

	DoActionDatas = InDoActionDatas;
}

void UCDoAction::DoAction()
{
	StateComponent->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;
}
