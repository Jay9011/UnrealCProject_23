#include "Components/CGuardComponent.h"

#include "CStateComponent.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Utilities/CheckMacros.h"

UCGuardComponent::UCGuardComponent()
{
}

void UCGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	
#if DEBUG_GUARD
	UDebuggerComponent* DebugCollector = Cast<UDebuggerComponent>(GetOwner()->GetComponentByClass(UDebuggerComponent::StaticClass()));
	if (DebugCollector != nullptr)
	{
		DebugCollector->AddCollector(this);
	}
#endif
}

void UCGuardComponent::InitGuardDelegate()
{
	OnEvaluateBlocking.Clear();
	OnEvaluateParrying.Clear();
}

void UCGuardComponent::EvaluateGuard(bool bSuccess, FDamagedData DamagedData)
{
	if (!bSuccess)
		return;

	switch (GuardType)
	{
	case EGuardType::Blocking:
		{
			if (OnEvaluateBlocking.IsBound())
			{
				OnEvaluateBlocking.Broadcast(DamagedData);
			}
		}
		break;
	case EGuardType::Parrying:
		{
			if (OnEvaluateParrying.IsBound())
			{
				OnEvaluateParrying.Broadcast(DamagedData);
			}
		}
		break;
	default: ;
	}
}

void UCGuardComponent::SetUnGuard()
{
	SetBlockingType(EGuardType::None);
}

void UCGuardComponent::SetBlockingMode()
{
	UCStateComponent* StateComponent = Cast<UCStateComponent>(GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(StateComponent);
	
	SetBlockingType(EGuardType::Blocking);
	
	if (!StateComponent->IsBlockingMode())
		StateComponent->SetBlockingMode();
}

void UCGuardComponent::SetParryingMode()
{
	SetBlockingType(EGuardType::Parrying);
}

void UCGuardComponent::SetBlockingType(EGuardType InBlockingType)
{
	GuardType = InBlockingType;

	if (OnGuardTypeChanged.IsBound())
	{
		OnGuardTypeChanged.Broadcast(GuardType);
	}
}

#if DEBUG_GUARD
bool UCGuardComponent::IsDebugEnable()
{
	return true;
}

FDebugInfo UCGuardComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;

	DebugInfo.Priority = static_cast<int32>(DEBUG_NUMS::GUARD);

	FString BlockingTypeString = "GuardType: " + StaticEnum<EGuardType>()->GetNameStringByValue(static_cast<uint8>(GuardType));
	DebugInfo.Data.Add({BlockingTypeString, FColor::Yellow});
	
	return DebugInfo;
}
#endif
