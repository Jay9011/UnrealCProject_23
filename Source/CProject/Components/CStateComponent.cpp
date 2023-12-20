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

#if DEBUG_STATE
	if(UDebuggerComponent* Debugger = GetOwner()->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
}

void UCStateComponent::ChangeType(EStateType InType)
{
	if (OnBeforeStateChange.IsBound())
		OnBeforeStateChange.Broadcast(Type, InType);
	
	EStateType PrevType = Type;
	Type = InType;
	
#if DEBUG_STATE
	UE_LOG(LogTemp, Warning, TEXT("%s Change State : %s -> %s"), *GetOwner()->GetName(), *StaticEnum<EStateType>()->GetNameStringByValue(static_cast<uint8>(PrevType)), *StaticEnum<EStateType>()->GetNameStringByValue(static_cast<uint8>(Type)));
#endif
	
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

#if DEBUG_STATE
FDebugInfo UCStateComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;
	DebugInfo.Priority = static_cast<int32>(DEBUG_NUMS::STATE);
	
	UEnum* StateEnum = StaticEnum<EStateType>();
	FString StateString = "State : " + StateEnum->GetNameStringByValue(static_cast<uint8>(Type));
	DebugInfo.Data.Add({StateString, FColor::Yellow});

	DebugInfo.Data.Add({"SubAction : " + FString(bInSubActionMode ? "true" : "false"), FColor::White});
	
	return DebugInfo;
}
#endif
