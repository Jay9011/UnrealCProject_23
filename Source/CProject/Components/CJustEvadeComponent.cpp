#include "Components/CJustEvadeComponent.h"

#include "Components/DecalComponent.h"
#include "GameFramework/Character.h"
#include "MyDebugger/DebuggerComponent.h"
#include "Parts/CGhostTrailComponent.h"
#include "Utilities/GTimeController.h"

UCJustEvadeComponent::UCJustEvadeComponent()
{
	
}

void UCJustEvadeComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if(OwnerCharacter != nullptr)
		GhostTrailComponent = Cast<UCGhostTrailComponent>(OwnerCharacter->GetComponentByClass(UCGhostTrailComponent::StaticClass()));

#if DEBUG_JUST_EVADE
	if (OwnerCharacter == nullptr)
		return;
	if(UDebuggerComponent* Debugger = OwnerCharacter->FindComponentByClass<UDebuggerComponent>())
		Debugger->AddCollector(this);
#endif
}

void UCJustEvadeComponent::StartJustEvade(const float InTimeDilation, const float InDuration,
                                          const bool UseGhostTrail, const float InTrailInterval, const float InTrailFirstDelay, const float InTrailDuration)
{
	JustTime = true;
	
	UGTimeController::SetTimeDilation(GetWorld(), InTimeDilation, InDuration);
	
	/*
	 * GhostTrail 사용 
	 */
	if (!UseGhostTrail)
		return;

	// 이전에 지연시간을 준 타이머가 존재한다면 중단한다.
	InitTimerHandle();
	
	if(InTrailFirstDelay > 0.f)
	{
		// 첫 번째 고스트 트레일을 그리기 전에 지연시간을 준다.
		FTimerDelegate GhostTrailTimerDelegate;
		GhostTrailTimerDelegate.BindLambda([=]()
		{
			GhostTrailComponent->Show(OwnerCharacter, InTrailInterval);
		});
		GetWorld()->GetTimerManager().SetTimer(DrawGhostTrailTimerHandle, GhostTrailTimerDelegate, InTrailFirstDelay, false);
	}
	else
	{
		GhostTrailComponent->Show(OwnerCharacter, InTrailInterval);
	}

	// 만약, InDuration이 존재한다면 지정된 시간 후에 고스트 트레일을 중단한다.
	if (InTrailDuration > 0.f)
	{
		FTimerDelegate GhostTrailTimerDelegate;
		GhostTrailTimerDelegate.BindLambda([=]()
		{
			GhostTrailComponent->Stop();
		});
		GetWorld()->GetTimerManager().SetTimer(StopGhostTrailTimerHandle, GhostTrailTimerDelegate, InTrailDuration, false);
	}
}

void UCJustEvadeComponent::EndJustEvade()
{
	JustTime = false;

	UGTimeController::SetTimeDilation(GetWorld(), 1.f);
	GhostTrailComponent->Stop();

	InitTimerHandle();
}

void UCJustEvadeComponent::InitTimerHandle()
{
	// 이전에 지연시간을 준 타이머가 존재한다면 중단한다.
	if (DrawGhostTrailTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(DrawGhostTrailTimerHandle);

	if (StopGhostTrailTimerHandle.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(StopGhostTrailTimerHandle);
}

#if DEBUG_JUST_EVADE
bool UCJustEvadeComponent::IsDebugEnable()
{
	return JustTime;
}

FDebugInfo UCJustEvadeComponent::GetDebugInfo()
{
	FDebugInfo DebugInfo;
	DebugInfo.Priority = static_cast<int32>(DEBUG_NUMS::JUST_EVADE);

	DebugInfo.Data.Add({"Enable JustTime!!!", FColor::Yellow});
	
	return DebugInfo;
}
#endif
