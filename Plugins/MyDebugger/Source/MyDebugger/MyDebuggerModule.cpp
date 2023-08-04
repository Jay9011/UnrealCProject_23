#include "MyDebuggerModule.h"
#include "DebuggerCategory.h"

#include "GameplayDebugger.h"

#define LOCTEXT_NAMESPACE "MyDebuggerModule"
IMPLEMENT_MODULE(FMyDebuggerModule, MyDebugger)

void FMyDebuggerModule::StartupModule()
{
	// FGameplayDebuggerCategory의 인스턴스를 생성할 수 있는 Delegate를 생성한다.
	IGameplayDebugger::FOnGetCategory Category;
	Category.BindStatic(&FDebuggerCategory::MakeInstance);

	// "Player"라는 이름으로 GameplayDebugger 싱글톤에 등록한 후 Category를 활성화한다.
	IGameplayDebugger::Get().RegisterCategory("Player", Category, EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);
	IGameplayDebugger::Get().NotifyCategoriesChanged();
}

void FMyDebuggerModule::ShutdownModule()
{
	// "Player"라는 이름의 Category를 GameplayDebugger 싱글톤에서 제거한다.
	if(IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("Player");
}

#undef LOCTEXT_NAMESPACE