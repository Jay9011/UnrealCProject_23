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
				|| !DebugInfoObject->IsDebugEnable())	// 디버깅 불가능한 상태라면 넘어간다.
					continue;
			
			DebugInfoList.Emplace(DebugInfoObject->GetDebugInfo());
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

	// 2D 직사각형 타일을 구성
	FVector2D TileSize(0, 0);
	FCanvasTileItem TileItem(FVector2D(10, 10), FVector2D(MaxTileWidth + 50, TotalTileHeight), FLinearColor(0, 0, 0, 0.2f));
	TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(TileItem, CanvasContext.CursorX, CanvasContext.CursorY);

	// 다음 계산을 위해 초기화
	MaxTileWidth = 0.f;
	TotalTileHeight = 0.f;

	// 데이터 텍스트를 출력한다.
	const FString PlayerName = L"Player: " + PlayerPawnData.Name;
	CanvasContext.Printf(FColor::Green, L"%s", *PlayerName);
	CanvasContext.MeasureString(PlayerName, TileSize.X, TileSize.Y);
	MaxTileWidth = FMath::Max(MaxTileWidth, TileSize.X);
	TotalTileHeight += TileSize.Y;

	const FString PlayerLocation = L"Location: " + PlayerPawnData.Location.ToString();
	CanvasContext.Printf(FColor::White, L"%s", *PlayerLocation);
	CanvasContext.MeasureString(PlayerLocation, TileSize.X, TileSize.Y);
	MaxTileWidth = FMath::Max(MaxTileWidth, TileSize.X);
	TotalTileHeight += TileSize.Y;

	// 데이터 리스트를 출력한다.
	for (const FDebugInfo& Info : DebugInfoList)
	{
		for (const FDebugInfo::FDebugText& Data : Info.Data)
		{
			CanvasContext.Printf(Data.Color, L"%s", *Data.Text);
			CanvasContext.MeasureString(Data.Text, TileSize.X, TileSize.Y);
			MaxTileWidth = FMath::Max(MaxTileWidth, TileSize.X);
			TotalTileHeight += TileSize.Y;
		}
		CanvasContext.Printf(FColor::White, L"---");
		CanvasContext.MeasureString(L"---", TileSize.X, TileSize.Y);
		TotalTileHeight += TileSize.Y;
	}
}