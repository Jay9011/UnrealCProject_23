#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "Interface/CDamagedInterface.h"
#include "CGuardComponent.generated.h"

UENUM(BlueprintType)
enum class EGuardType : uint8
{
	None,
	Blocking,
	Parrying,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGuardTypeChanged, EGuardType, InBlockingType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEvaluateBlocking, FDamagedData&, DamagedData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEvaluateParrying, FDamagedData&, DamagedData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCGuardComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCGuardComponent();

protected:
	virtual void BeginPlay() override;

public:
	void InitGuardDelegate();
	
public:
	void EvaluateGuard(bool bSuccess, FDamagedData DamagedData);
	
public:
	void SetUnGuard();
	void SetBlockingMode();
	void SetParryingMode();

private:
	void SetBlockingType(EGuardType InBlockingType);
	
public:
	FGuardTypeChanged OnGuardTypeChanged;
	FEvaluateBlocking OnEvaluateBlocking;
	FEvaluateParrying OnEvaluateParrying;
	
private:
	EGuardType GuardType = EGuardType::None;

/*
 * Getter
 */
public:
	FORCEINLINE EGuardType GetGuardType() const { return GuardType; }
	FORCEINLINE bool IsUnGuard() const { return GuardType == EGuardType::None; }
	FORCEINLINE bool IsBlocking() const { return GuardType == EGuardType::Blocking; }
	FORCEINLINE bool IsParrying() const { return GuardType == EGuardType::Parrying; }

#if DEBUG_GUARD
public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
