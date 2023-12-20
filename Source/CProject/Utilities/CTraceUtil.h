#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
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
	                                           FHitResult& OutHitResult, bool bDebug = false, EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None, float Duration = 0.f);
	
	static bool TraceForwardOtherGroupByProfile(ACharacter* InOwnerCharacter, FVector InStart, FVector InEnd, float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors,
												TArray<FHitResult>& OutHitResults, bool bDebug = false, EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None, float Duration = 0.f);

	static bool SingleTraceSphereOtherGroupForObjects(ACharacter* InOwnerCharacter, float InRadius, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& InIgnoreActors,
											   FHitResult& OutHitResults, bool bDebug = false, EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None, float Duration = 0.f);

	static bool SingleTraceSphereOtherGroupTraceChannel(ACharacter* InOwnerCharacter, float InRadius, const TEnumAsByte<ETraceTypeQuery> TraceChannel, const TArray<AActor*>& InIgnoreActors,
													   FHitResult& OutHitResults, bool bDebug = false, EDrawDebugTrace::Type DebugTrace = EDrawDebugTrace::None, float Duration = 0.f);
};
