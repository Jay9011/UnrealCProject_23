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
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
		FString State;
	};

private:
	FCategoryData PlayerPawnData;
};
