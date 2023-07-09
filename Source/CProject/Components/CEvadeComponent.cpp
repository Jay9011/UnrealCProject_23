#include "Components/CEvadeComponent.h"
#include "Global.h"

#include "Character/CBaseCharacter.h"
#include "CMovementComponent.h"
#include "CStateComponent.h"

UCEvadeComponent::UCEvadeComponent()
{
	Owner = Cast<ACBaseCharacter>(GetOwner());

	if(!!Owner)
	{
		Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
		State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	}
}

void UCEvadeComponent::BeginPlay()
{
	Super::BeginPlay();

	//TArray에 들어있는 FEvadeData를 순회하면서 EEvadeDir의 순서에 맞춰 재정렬 해준다.
	EvadeDataPtr.SetNum(static_cast<int32>(EEvdeDir::MAX));
	for(int32 i = 0; i < static_cast<int32>(EEvdeDir::MAX); i++)
	{
		if(EvadeData.Num() <= 0) break;
		for(FEvadeData& Data : EvadeData)
		{
			if(static_cast<EEvdeDir>(i) == Data.Dir)
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

void UCEvadeComponent::Evade_Implementation()
{
	IIEvadeAction::Evade_Implementation();

	EvadeDataPtr[static_cast<int32>(EEvdeDir::FRONT)]->PlayMontage(Owner);
}

void UCEvadeComponent::EndEvade_Implementation()
{
	IIEvadeAction::EndEvade_Implementation();

	State->SetIdleMode();
}
