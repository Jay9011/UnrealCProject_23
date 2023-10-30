#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

class MYDEBUGGER_API FDebuggerCategory
	: public FGameplayDebuggerCategory
{
public:
	FDebuggerCategory();
	virtual ~FDebuggerCategory() override;

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	// 디버깅 데이터 수집
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	// 디버깅 출력
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	// 디버깅 데이터
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
		FString State;
		bool SubAction;
		FString WeaponType;
		TArray<FString> WeaponAssetDebugInfo;
		TArray<FString> DoActionDebugInfo;
	};

private:
	FCategoryData PlayerPawnData;
};
