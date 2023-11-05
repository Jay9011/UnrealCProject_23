#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDebugCollector.generated.h"

/**
 * @struct FDebugInfo
 * @brief 디버깅 데이터
 */
struct FDebugInfo
{
	/**
	 * @struct FDebugText
	 * @brief 디버깅 텍스트 
	 */
	struct FDebugText
	{
		FColor Color = FColor::White; /// 텍스트 색상
		FString Text; /// 텍스트 내용

		FDebugText(FString InText, FColor InColor = FColor::White)
			: Color(InColor), Text(InText)
		{}
	};
	
	int32 Priority; /// 디버깅 데이터 우선순위
	TArray<FDebugText> Data; /// 디버깅 데이터

	FDebugInfo()
		: Priority(0)
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
