#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "CJustEvadeComponent.generated.h"

class UCGhostTrailComponent;
class ACGhostTrail;
/*
 * @breif JustEvade 관련 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCJustEvadeComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCJustEvadeComponent();

protected:
	virtual void BeginPlay() override;

public:
	void InitTimerHandle();

	void StartJustEvade(const float InTimeDilation, const float InDuration, const bool UseGhostTrail = false, const float InTrailInterval = -1.f, const float InTrailFirstDelay = -1.f, const float InTrailDuration = 0.0f);
	void EndJustEvade();

private:
	bool JustTime = false;

	UPROPERTY()
	UCGhostTrailComponent* GhostTrailComponent = nullptr;
	
	FTimerHandle DrawGhostTrailTimerHandle;
	FTimerHandle StopGhostTrailTimerHandle;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

/*
 * Getter / Setter
 */
public:
	FORCEINLINE bool IsJustTime() const { return JustTime; }
	FORCEINLINE void SetJustTime(bool InJustTime) { JustTime = InJustTime; }

	FORCEINLINE ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }
	FORCEINLINE void SetOwnerCharacter(ACharacter* InOwnerCharacter) { OwnerCharacter = InOwnerCharacter; }

#if DEBUG_JUST_EVADE
public:
	virtual bool IsDebugEnable();
	virtual FDebugInfo GetDebugInfo();
#endif
};
