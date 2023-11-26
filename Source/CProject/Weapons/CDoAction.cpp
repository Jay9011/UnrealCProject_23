#include "Weapons/CDoAction.h"

#include "CWeaponAsset.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

#define DebugFlag 1

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(UCWeaponAsset* InOwnerWeaponAsset, ACharacter* InOwner)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld(); //UObject는 World가 없으므로, OwnerCharacter의 World를 가져온다.

	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

	OwnerWeaponAsset = InOwnerWeaponAsset;

	InitDoActionData();

	BP_BeginPlay();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
	OwnerWeaponAsset->UpdateActions();
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	StateComponent->SetIdleMode();

	MovementComponent->Move();
	MovementComponent->DisableFixedCamera();

	OwnerWeaponAsset->UpdateActions();
}

UCDoAction* UCDoAction::BP_AddDoAction(TSubclassOf<UCDoAction> InDoActionClass)
{
	UCDoAction* NewDoAction = NewObject<UCDoAction>(this, InDoActionClass);
	NewDoAction->BeginPlay(OwnerWeaponAsset, OwnerCharacter);

	return NewDoAction;
}

void UCDoAction::DoAction_Implementation()
{
	StateComponent->SetActionMode();
	OwnerWeaponAsset->ReserveAction(this);
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
