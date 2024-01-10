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
	if (!!PlayerPawn)
	{
		DebugInfoList.Empty();
		// ACharacter의 DebuggerComponent를 가져온다.
		UDebuggerComponent* DebuggerComponent = PlayerPawn->FindComponentByClass<UDebuggerComponent>();
		if (!!DebuggerComponent)
		{
			PlayerPawnData.bDraw = true;
			PlayerPawnData.Name = PlayerPawn->GetName();
			PlayerPawnData.Location = PlayerPawn->GetActorLocation();

			GatherDebugInfo(PlayerPawn, DebuggerComponent, DebugInfoList);
		}
	}

	if (!!DebugActor)
	{
		TestDebugInfoList.Empty();
		
		UDebuggerComponent* DebuggerComponent = DebugActor->FindComponentByClass<UDebuggerComponent>();
		if (!!DebuggerComponent)
		{
			DebuggerActorData.bDraw = true;
			DebuggerActorData.Name = DebugActor->GetName();

			GatherDebugInfo(DebugActor, DebuggerComponent, TestDebugInfoList);
		}
	}
}

void FDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	/*
	 * Player Actor용 데이터를 출력한다.
	 */
	if (PlayerPawnData.bDraw)
	{
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

	/*
	 * Debugger Actor용 데이터를 출력한다.
	 */
	if (DebuggerActorData.bDraw)
	{
		// 2D 직사각형 타일을 다음 위치에 구성
		CanvasContext.CursorY += 10.f;
		FVector2D TargetTileSize(0, 0);
		FCanvasTileItem TargetTileItem(FVector2D(10, CanvasContext.CursorY), FVector2D(TargetMaxTileWidth + 50, TargetTotalTileHeight), FLinearColor(0, 0, 0, 0.2f));
		TargetTileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
		CanvasContext.DrawItem(TargetTileItem, CanvasContext.CursorX, CanvasContext.CursorY);

		// 다음 계산을 위해 초기화
		TargetMaxTileWidth = 0.f;
		TargetTotalTileHeight = 0.f;

		// 데이터 텍스트를 출력한다.
		const FString TargetName = L"Target: " + DebuggerActorData.Name;
		CanvasContext.Printf(FColor::Green, L"%s", *TargetName);
		CanvasContext.MeasureString(TargetName, TargetTileSize.X, TargetTileSize.Y);
		TargetMaxTileWidth = FMath::Max(TargetMaxTileWidth, TargetTileSize.X);
		TargetTotalTileHeight += TargetTileSize.Y;

		// 데이터 리스트를 출력한다.
		for (const FDebugInfo& Info : TestDebugInfoList)
		{
			for (const FDebugInfo::FDebugText& Data : Info.Data)
			{
				CanvasContext.Printf(Data.Color, L"%s", *Data.Text);
				CanvasContext.MeasureString(Data.Text, TargetTileSize.X, TargetTileSize.Y);
				TargetMaxTileWidth = FMath::Max(TargetMaxTileWidth, TargetTileSize.X);
				TargetTotalTileHeight += TargetTileSize.Y;
			}
			CanvasContext.Printf(FColor::White, L"---");
			CanvasContext.MeasureString(L"---", TargetTileSize.X, TargetTileSize.Y);
			TargetTotalTileHeight += TargetTileSize.Y;
		}
	}
}

void FDebuggerCategory::GatherDebugInfo(const AActor* InActor, UDebuggerComponent* InDebuggerComponent, TArray<FDebugInfo>& OutDebugInfoList)
{
	if (!InDebuggerComponent)
		return;
	
	for (const TWeakObjectPtr<UObject>& DebugInfo : InDebuggerComponent->GetCollectors())
	{
		if (DebugInfo.IsValid())
		{
			IIDebugCollector* DebugInfoObject = Cast<IIDebugCollector>(DebugInfo.Get());

			if (DebugInfoObject == nullptr
				|| !DebugInfoObject->IsDebugEnable())	// 디버깅 불가능한 상태라면 넘어간다.
				continue;

			OutDebugInfoList.Emplace(DebugInfoObject->GetDebugInfo());
		}
		else
		{
			InDebuggerComponent->RemoveCollector(Cast<IIDebugCollector>(DebugInfo.Get()));
		}
	}

	OutDebugInfoList.Sort();
}
