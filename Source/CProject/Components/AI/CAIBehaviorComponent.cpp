#include "Components/AI/CAIBehaviorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
}

void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

}

EAIStateType UCAIBehaviorComponent::GetAIStateType() const
{
	return static_cast<EAIStateType>(Blackboard->GetValueAsEnum(AIStateTypeKey));
}

void UCAIBehaviorComponent::ChangeAIState(EAIStateType InType)
{
	EAIStateType PrevType = GetAIStateType();

	Blackboard->SetValueAsEnum(AIStateTypeKey, static_cast<uint8>(InType));

	if (OnAIStateChanged.IsBound())
		OnAIStateChanged.Broadcast(PrevType, InType);
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

FVector UCAIBehaviorComponent::GetPatrolLocation() const
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}

bool UCAIBehaviorComponent::IsWait() const
{
	return GetAIStateType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsPatrol() const
{
	return GetAIStateType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsApproach() const
{
	return GetAIStateType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsAction() const
{
	return GetAIStateType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsHitted() const
{
	return GetAIStateType() == EAIStateType::Hitted;
}

bool UCAIBehaviorComponent::IsEvade() const
{
	return GetAIStateType() == EAIStateType::Evade;
}

bool UCAIBehaviorComponent::IsDead() const
{
	return GetAIStateType() == EAIStateType::Dead;
}

void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeAIState(EAIStateType::Wait);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeAIState(EAIStateType::Patrol);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeAIState(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeAIState(EAIStateType::Action);
}

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeAIState(EAIStateType::Hitted);
}

void UCAIBehaviorComponent::SetEvadeMode()
{
	ChangeAIState(EAIStateType::Evade);
}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeAIState(EAIStateType::Dead);
}
