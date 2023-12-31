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
	OnGuardSuccess.Clear();
	OnParryingSuccess.Clear();
}

void UCGuardComponent::GuardSuccess(bool bSuccess, FDamagedData DamagedData)
{
	if (!bSuccess)
		return;

	switch (BlockingType)
	{
	case EBlockingType::Blocking:
		{
			if (OnGuardSuccess.IsBound())
			{
				OnGuardSuccess.Broadcast(bSuccess, DamagedData);
			}
		}
		break;
	case EBlockingType::Parrying:
		{
			if (OnParryingSuccess.IsBound())
			{
				OnParryingSuccess.Broadcast(bSuccess, DamagedData);
			}
		}
		break;
	default: ;
	}
}

void UCGuardComponent::SetUnBlocking()
{
	SetBlockingType(EBlockingType::None);
}

void UCGuardComponent::SetBlockingMode()
{
	UCStateComponent* StateComponent = Cast<UCStateComponent>(GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(StateComponent);
	
	SetBlockingType(EBlockingType::Blocking);
	
	if (!StateComponent->IsBlockingMode())
		StateComponent->SetBlockingMode();
}

void UCGuardComponent::SetParryingMode()
{
	SetBlockingType(EBlockingType::Parrying);
}

void UCGuardComponent::SetBlockingType(EBlockingType InBlockingType)
{
	BlockingType = InBlockingType;

	if (OnBlockingTypeChanged.IsBound())
	{
		OnBlockingTypeChanged.Broadcast(BlockingType);
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

	FString BlockingTypeString = "BlockingType: " + StaticEnum<EBlockingType>()->GetNameStringByValue(static_cast<uint8>(BlockingType));
	DebugInfo.Data.Add({BlockingTypeString, FColor::Yellow});
	
	return DebugInfo;
}
#endif
