#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "Interface/CDamagedInterface.h"
#include "CGuardComponent.generated.h"

UENUM(BlueprintType)
enum class EBlockingType : uint8
{
	None,
	Blocking,
	Parrying,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlockingTypeChanged, EBlockingType, InBlockingType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlockingSuccess, bool, bSuccess, FDamagedData&, DamagedData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FParryingSuccess, bool, bSuccess, FDamagedData&, DamagedData);

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
	void GuardSuccess(bool bSuccess, FDamagedData DamagedData);
	
public:
	void SetUnBlocking();
	void SetBlockingMode();
	void SetParryingMode();

private:
	void SetBlockingType(EBlockingType InBlockingType);
	
public:
	FBlockingTypeChanged OnBlockingTypeChanged;
	FBlockingSuccess OnGuardSuccess;
	FParryingSuccess OnParryingSuccess;
	
private:
	EBlockingType BlockingType = EBlockingType::None;

/*
 * Getter
 */
public:
	FORCEINLINE EBlockingType GetBlockingType() const { return BlockingType; }
	FORCEINLINE bool IsUnBlocking() const { return BlockingType == EBlockingType::None; }
	FORCEINLINE bool IsBlocking() const { return BlockingType == EBlockingType::Blocking; }
	FORCEINLINE bool IsParrying() const { return BlockingType == EBlockingType::Parrying; }

#if DEBUG_GUARD
public:
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;
#endif
};
