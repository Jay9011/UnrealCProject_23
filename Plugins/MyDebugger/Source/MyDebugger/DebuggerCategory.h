#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"
#include "IDebugCollector.h"

class MYDEBUGGER_API FDebuggerCategory
	: public FGameplayDebuggerCategory
{
public:
	FDebuggerCategory();
	virtual ~FDebuggerCategory() override;

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	// 디버깅 데이터 수집용 액터를 등록한다.
	void RegistDebugObject(UObject* DebugObject);
	void UnregistDebugObject(UObject* DebugObject);
	
private:
	// 디버깅 데이터 수집
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	// 디버깅 출력
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	// 수집된 데이터 리스트
	TArray<FDebugInfo> DebugInfoList;
	// 데이터 수집용 액터 리스트
	TArray<TWeakObjectPtr<UObject>> DebugObjects;

private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
	} PlayerPawnData;
};
