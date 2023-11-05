#include "Components/CStateComponent.h"

#include "MyDebugger/DebuggerCategory.h"
#include "MyDebugger/DebuggerComponent.h"
#include "MyDebugger/MyDebuggerModule.h"

UCStateComponent::UCStateComponent()
{

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if(UDebuggerComponent* Debugger = GetOwner()->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
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

#if WITH_EDITOR
FDebugInfo UCStateComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;
	DebugInfo.Priority = 0;
	DebugInfo.Color = FColor::Red;

	UEnum* StateEnum = StaticEnum<EStateType>();
	FString StateString = "State : " + StateEnum->GetNameStringByValue(static_cast<uint8>(Type));
	DebugInfo.Data.Add(StateString);

	return DebugInfo;
}
#endif
