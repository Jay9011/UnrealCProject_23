#include "Components/CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType PrevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(PrevType, Type);
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}
