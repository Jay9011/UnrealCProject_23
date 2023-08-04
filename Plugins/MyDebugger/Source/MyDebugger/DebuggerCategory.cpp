#include "DebuggerCategory.h"

#include "CanvasItem.h"
#include "Character/CPlayer.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/IDoActionDebugData.h"
#include "Weapons/CDoAction.h"

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
	
	const ACPlayer* CPlayer = Cast<ACPlayer>(PlayerPawn);
	// Enum을 String으로 변환하기 위해 UEnum을 가져온다.
	const UEnum* EnumState = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
	const UEnum* EnumWeapon = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	//Player
	{
		PlayerPawnData.bDraw = true;
		PlayerPawnData.Name = PlayerPawn->GetName();
		PlayerPawnData.Location = PlayerPawn->GetActorLocation();

		if (!!EnumState)
		{
			//EStateType을 UEnum으로 가져올 수 있다면, StateComponent의 State를 가져와서 String으로 변환한다. 
			UCStateComponent* State = Cast<UCStateComponent>(CPlayer->GetComponentByClass(UCStateComponent::StaticClass()));
			FString StateString = EnumState->GetDisplayNameTextByIndex(static_cast<int64>(State->GetType())).ToString();
			PlayerPawnData.State = StateString;
		}

		if(!!EnumWeapon)
		{
			//EWeaponType을 UEnum으로 가져올 수 있다면, WeaponComponent의 Type을 가져와서 String으로 변환한다.
			UCWeaponComponent* Weapon = Cast<UCWeaponComponent>(CPlayer->GetComponentByClass(UCWeaponComponent::StaticClass()));
			FString WeaponString = EnumWeapon->GetDisplayNameTextByIndex(static_cast<int64>(Weapon->GetType())).ToString();
			PlayerPawnData.WeaponType = WeaponString;
			
			//각 무기의 DoAction에 대한 디버깅 정보를 가져온다.
			IIDoActionDebugData* DoActionDebugData = Cast<IIDoActionDebugData>(Weapon->GetDoAction());
			if(!!DoActionDebugData)
			{
				PlayerPawnData.DoActionDebugInfo = DoActionDebugData->GetDebugInfo();
			}
			else
			{
				PlayerPawnData.DoActionDebugInfo.Empty();
			}
		}
	}
}

void FDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	// 2D 직사각형 타일을 그린다.
	FCanvasTileItem TileItem(FVector2D(10, 10), FVector2D(300, 215), FLinearColor(0, 0, 0, 0.2f));
	TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(TileItem, CanvasContext.CursorX, CanvasContext.CursorY);

	// 데이터 텍스트를 출력한다.
	CanvasContext.Printf(FColor::Green, L"Player: %s", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L"Location: %s", *PlayerPawnData.Location.ToString());
	CanvasContext.Printf(FColor::Red, L"State: %s", *PlayerPawnData.State);
	CanvasContext.Printf(FColor::White, L"---");
	CanvasContext.Printf(FColor::Red, L"Weapon: %s", *PlayerPawnData.WeaponType);
	for (const FString& DebugInfo : PlayerPawnData.DoActionDebugInfo)
	{
		CanvasContext.Printf(FColor::Red, L"%s", *DebugInfo);
	}
	CanvasContext.Printf(FColor::White, L"---");
}
