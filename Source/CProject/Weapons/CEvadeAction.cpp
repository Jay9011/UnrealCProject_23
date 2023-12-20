#include "Weapons/CEvadeAction.h"

#include "Components/CEvadeComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

void UCEvadeAction::BeginPlay(ACharacter* InOwner, TArray<FEvadeData>& InData)
{
	OwnerCharacter = InOwner;
	
	StateComponent = Cast<UCStateComponent>(InOwner->GetComponentByClass(UCStateComponent::StaticClass()));
	EvadeComponent = Cast<UCEvadeComponent>(InOwner->GetComponentByClass(UCEvadeComponent::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	// Evade 상태에서 상태 변경시 Evade에서 설정한 설정을 되돌려야 한다.
	if (StateComponent != nullptr)
	{
		StateComponent->OnBeforeStateChange.AddDynamic(this, &UCEvadeAction::OnBeforeStateChange);
	}
	
	//TArray에 들어있는 FEvadeData를 순회하면서 EEvadeDir의 순서에 맞춰 재정렬 해준다.
	EvadeDataPtr.SetNum(static_cast<int32>(EEvadeDir::MAX));
	for(int32 i = 0; i < static_cast<int32>(EEvadeDir::MAX); i++)
	{
		if(InData.Num() <= 0) break;
		for(FEvadeData& Data : InData)
		{
			if(static_cast<EEvadeDir>(i) == Data.Dir)
			{
				EvadeDataPtr[i] = &Data;
				break;
			}
		}// for(FEvadeData& Data : EvadeData)
	}
}

void UCEvadeAction::OnBeginEquip()
{
}

void UCEvadeAction::OnUnequip()
{
}

void UCEvadeAction::BeginEvade()
{
}

void UCEvadeAction::EndEvade()
{
	StateComponent->SetIdleMode();
}

void UCEvadeAction::OnBeforeStateChange(EStateType InPrevState, EStateType InNewState)
{
	if (InPrevState == EStateType::Evade)
	{
		if (InNewState != EStateType::Idle)
			EndEvade();
	}
		
}
