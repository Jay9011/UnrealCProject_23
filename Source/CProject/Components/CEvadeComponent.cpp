#include "Components/CEvadeComponent.h"
#include "Global.h"

#include "Character/CBaseCharacter.h"
#include "CMovementComponent.h"
#include "CStateComponent.h"

UCEvadeComponent::UCEvadeComponent()
{
}

void UCEvadeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<ACBaseCharacter>(GetOwner());
	if(!!Owner)
	{
		Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
		State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	}

	DelegateInit();
	
	//TArray에 들어있는 FEvadeData를 순회하면서 EEvadeDir의 순서에 맞춰 재정렬 해준다.
	EvadeDataPtr.SetNum(static_cast<int32>(EEvadeDir::MAX));
	for(int32 i = 0; i < static_cast<int32>(EEvadeDir::MAX); i++)
	{
		if(EvadeData.Num() <= 0) break;
		for(FEvadeData& Data : EvadeData)
		{
			if(static_cast<EEvadeDir>(i) == Data.Dir)
			{
				EvadeDataPtr[i] = &Data;
				break;
			}
		}// for(FEvadeData& Data : EvadeData)
	}
	
}

void UCEvadeComponent::OnEvade()
{
	CheckFalse(State->IsIdleMode())
	CheckFalse(Movement->CanMove())

	State->SetEvadeMode();
}

void UCEvadeComponent::OnBeginEvade()
{
	EvadeDataPtr[static_cast<int32>(EEvadeDir::FRONT)]->PlayMontage(Owner);
}

void UCEvadeComponent::OnEndEvade()
{
	State->SetIdleMode();
}

void UCEvadeComponent::DelegateInit()
{
	OnBeginEvadeDelegate.Clear();
	OnEndEvadeDelegate.Clear();
	OnBeginEvadeDelegate.AddDynamic(this, &UCEvadeComponent::OnBeginEvade);
	OnEndEvadeDelegate.AddDynamic(this, &UCEvadeComponent::OnEndEvade);
}
