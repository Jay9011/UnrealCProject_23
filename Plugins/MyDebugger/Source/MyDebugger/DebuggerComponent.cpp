#include "DebuggerComponent.h"
#include "MyDebugger/IDebugCollector.h"

UDebuggerComponent::UDebuggerComponent()
{

}

void UDebuggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UDebuggerComponent::AddCollector(IIDebugCollector* InCollector)
{
	TWeakObjectPtr<UObject> Collector = Cast<UObject>(InCollector);
	if (Collector.IsValid())
	{
		Collectors.AddUnique(Collector);
		return true;
	}

	return false;
}

bool UDebuggerComponent::RemoveCollector(IIDebugCollector* InCollector)
{
	TWeakObjectPtr<UObject> Collector = Cast<UObject>(InCollector);
	if (Collector.IsValid())
	{
		Collectors.Remove(Collector);
		return true;
	}

	return false;
}