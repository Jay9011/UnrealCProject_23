#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "IDebugCollector.h"
#include "MyDebuggerModule.h"

class MYDEBUGGER_API FDebuggerCategory
	: public FGameplayDebuggerCategory
{
public:
	FDebuggerCategory();
	virtual ~FDebuggerCategory() override;

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

private:
	// 디버깅 데이터 수집
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	// 디버깅 출력
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	void GatherDebugInfo(const AActor* InActor, class UDebuggerComponent* InDebuggerComponent, TArray<FDebugInfo>& OutDebugInfoList);
	
private:
	// 수집된 데이터 리스트
	TArray<FDebugInfo> DebugInfoList;
	TArray<FDebugInfo> TestDebugInfoList;

private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
	} PlayerPawnData;

	struct FDebuggerActorData
	{
		bool bDraw = false;
		FString Name;
	} DebuggerActorData;

	float MaxTileWidth = 0.f;
	float TotalTileHeight = 0.f;

	float TargetMaxTileWidth = 0.f;
	float TargetTotalTileHeight = 0.f;
};
