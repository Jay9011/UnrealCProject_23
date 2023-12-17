#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GTimeController.generated.h"

/**
 * @brief 시간과 관련된 함수들
 */
UCLASS()
class CPROJECT_API UGTimeController : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void SetTimeDilation(const class UWorld* InWorld, float InTimeDilation, float InDuration = 0, const TArray<AActor*>& InIgnoreActors = TArray<AActor*>());
	
	UFUNCTION(BlueprintCallable, Category = "Time")
	static void SetTimeDilationOnlyActors(class UWorld* InWorld, float InTimeDilation, float InDuration);

	UFUNCTION(BlueprintCallable, Category = "Time")
	static void SetTimeDilationOnlyActorsAddIgnoreActors(UWorld* InWorld, float InTimeDilation, float InDuration, TArray<AActor*>& InIgnoreActors);
};
