#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DebuggerComponent.generated.h"


class IIDebugCollector;

UCLASS(NotBlueprintable, ClassGroup=(Debugger), meta=(BlueprintSpawnableComponent))
class MYDEBUGGER_API UDebuggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDebuggerComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool AddCollector(IIDebugCollector* InCollector);
	bool RemoveCollector(IIDebugCollector* InCollector);

public:
	// 디버깅 데이터 수집자 목록을 가져옵니다.
	const TArray<TWeakObjectPtr<UObject>>& GetCollectors() const { return Collectors; }
	
private:
	TArray<TWeakObjectPtr<UObject>> Collectors;
};
