#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDebugCollector.generated.h"

/**
 * @brief 디버깅 데이터
 * @param Priority 출력 우선순위
 * @param bCollect 데이터 수집 여부
 * @param Color 디버깅 색상
 * @param Data 디버깅 데이터
 */
struct FDebugInfo
{
	int32 Priority;
	FColor Color;
	TArray<FString> Data;

	FDebugInfo()
		: Priority(0) , Color(FColor::White)
	{}

	bool operator<(const FDebugInfo& Other) const
	{
		return Priority < Other.Priority;
	}
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDebugCollector : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief 디버그 데이터 인터페이스
 */
class MYDEBUGGER_API IIDebugCollector
{
	GENERATED_BODY()

public:
	virtual bool IsDebugEnable() = 0;
	virtual FDebugInfo GetDebugInfo() = 0;
};
