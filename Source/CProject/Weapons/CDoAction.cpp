#include "Weapons/CDoAction.h"

#include "CWeaponObject.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(ACharacter* InOwner, class UCWeaponObject* InWeapon)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld(); //UObject는 World가 없으므로, OwnerCharacter의 World를 가져온다.

	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

	Weapon = InWeapon;

	InitDoActionData();

	BP_BeginPlay();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
	Weapon->UpdateActions();
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	StateComponent->SetIdleMode();

	MovementComponent->Move();
	MovementComponent->DisableFixedCamera();

	Weapon->UpdateActions();
}

UCDoAction* UCDoAction::BP_AddDoAction(TSubclassOf<UCDoAction> InDoActionClass)
{
	UCDoAction* NewDoAction = NewObject<UCDoAction>(this, InDoActionClass);
	NewDoAction->BeginPlay(OwnerCharacter, Weapon);

	return NewDoAction;
}

void UCDoAction::DoAction_Implementation()
{
	StateComponent->SetActionMode();
	Weapon->ReserveAction(this);
}

void UCDoAction::InitDoActionData()
{
	if (!!ActionDataTable)
	{
		TArray<FDoActionData*> DoActionData;
		ActionDataTable->GetAllRows<FDoActionData>("", DoActionData);
		
		for (FDoActionData* Data : DoActionData)
			DoActionDatas.Add(*Data);		
	}
	else if (DoActionDatas.Num() == 0)
	{
		GLog->Log(ELogVerbosity::Error, TEXT("DoActionDatas is empty"));
	}
}
