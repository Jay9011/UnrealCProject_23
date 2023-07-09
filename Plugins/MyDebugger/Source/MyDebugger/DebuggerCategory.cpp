#include "DebuggerCategory.h"

#include "CanvasItem.h"
#include "Character/CPlayer.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

FDebuggerCategory::FDebuggerCategory()
{
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
	const ACPlayer* CPlayer = Cast<ACPlayer>(PlayerPawn);
	const UEnum* EnumState = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
	
	if(PlayerPawn == nullptr) return;

	//Player
	{
		PlayerPawnData.bDraw = true;
		PlayerPawnData.Name = PlayerPawn->GetName();
		PlayerPawnData.Location = PlayerPawn->GetActorLocation();

		if (EnumState != nullptr)
		{
			UCStateComponent* State = Cast<UCStateComponent>(CPlayer->GetComponentByClass(UCStateComponent::StaticClass()));
			FString StateString = EnumState->GetDisplayNameTextByIndex((int64)State->GetType()).ToString();
			PlayerPawnData.State = StateString;
		}
	}
}

void FDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	FCanvasTileItem TileItem(FVector2D(10, 10), FVector2D(300, 215), FLinearColor(0, 0, 0, 0.2f));
	TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(TileItem, CanvasContext.CursorX, CanvasContext.CursorY);

	CanvasContext.Printf(FColor::Green, L"Player: %s", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L"Location: %s", *PlayerPawnData.Location.ToString());
	CanvasContext.Printf(FColor::Red, L"State: %s", *PlayerPawnData.State);
	CanvasContext.Printf(FColor::White, L"");
}
