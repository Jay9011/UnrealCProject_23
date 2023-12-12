#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CTraceUtil.generated.h"

/**
 * @brief Trace 관련 유틸리티
 */
UCLASS()
class CPROJECT_API UCTraceUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static bool TraceForwardNearEnemyByProfile(ACharacter* InOwnerCharacter, float InLength, float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors,
	                                           FHitResult& OutHitResult);
	static bool TraceForwardOtherGroupByProfile(ACharacter* InOwnerCharacter, FVector InStart, FVector InEnd, float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors,
												TArray<FHitResult>& OutHitResults);
};
