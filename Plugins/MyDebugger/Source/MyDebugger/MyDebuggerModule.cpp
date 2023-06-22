#include "MyDebuggerModule.h"
#include "DebuggerCategory.h"

#include "GameplayDebugger.h"

#define LOCTEXT_NAMESPACE "MyDebuggerModule"
IMPLEMENT_MODULE(FMyDebuggerModule, MyDebugger)

void FMyDebuggerModule::StartupModule()
{
	IGameplayDebugger::FOnGetCategory Category;
	Category.BindStatic(&FDebuggerCategory::MakeInstance);
	
	IGameplayDebugger::Get().RegisterCategory("Player", Category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);
	IGameplayDebugger::Get().NotifyCategoriesChanged();
}

void FMyDebuggerModule::ShutdownModule()
{
	if(IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Player");
}

#undef LOCTEXT_NAMESPACE