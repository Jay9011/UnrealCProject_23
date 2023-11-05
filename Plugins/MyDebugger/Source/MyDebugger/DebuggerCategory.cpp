#include "DebuggerCategory.h"

#include "CanvasItem.h"
#include "MyDebugger/IDebugCollector.h"
#include "MyDebugger/DebuggerComponent.h"
#include "GameFramework/Character.h"

FDebuggerCategory::FDebuggerCategory()
{
	// Player만 지정해서 디버깅 할 수 있도록 지정되지 않은 액터를 디버깅 할 수 있게 한다.
	bShowOnlyWithDebugActor = false;
}

FDebuggerCategory::~FDebuggerCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FDebuggerCategory());
}

void FDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);

	const ACharacter* PlayerPawn = OwnerPC->GetPawn<ACharacter>();

	if(PlayerPawn == nullptr) return;
	PlayerPawnData.bDraw = true;
	PlayerPawnData.Name = PlayerPawn->GetName();
	PlayerPawnData.Location = PlayerPawn->GetActorLocation();

	DebugInfoList.Empty();
	// ACharacter의 DebuggerComponent를 가져온다.
	UDebuggerComponent* DebuggerComponent = PlayerPawn->FindComponentByClass<UDebuggerComponent>();
	if(DebuggerComponent == nullptr) return;
	
	for(const TWeakObjectPtr<UObject>& DebugInfo : DebuggerComponent->GetCollectors())
	{
		if (DebugInfo.IsValid())
		{
			IIDebugCollector* DebugInfoObject = Cast<IIDebugCollector>(DebugInfo.Get());
			
			if (DebugInfoObject == nullptr
				&& !DebugInfoObject->IsDebugEnable())	// 디버깅 불가능한 상태라면 넘어간다.
					continue;
			
			DebugInfoList.Add(DebugInfoObject->GetDebugInfo());
		}
		else
		{
			DebuggerComponent->RemoveCollector(Cast<IIDebugCollector>(DebugInfo.Get()));
		}
	}

	DebugInfoList.Sort();
}

void FDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	// 2D 직사각형 타일을 그린다.
	FCanvasTileItem TileItem(FVector2D(10, 10), FVector2D(300, 250), FLinearColor(0, 0, 0, 0.2f));
	TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(TileItem, CanvasContext.CursorX, CanvasContext.CursorY);

	// 데이터 텍스트를 출력한다.
	CanvasContext.Printf(FColor::Green, L"Player: %s", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L"Location: %s", *PlayerPawnData.Location.ToString());

	// 데이터 리스트를 출력한다.
	for (FDebugInfo& Info : DebugInfoList)
	{
		FColor Color = Info.Color;
		for (FString& DataString : Info.Data)
		{
			CanvasContext.Printf(Color, L"%s", *DataString);
		}
	}
}