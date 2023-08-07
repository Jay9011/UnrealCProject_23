#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEvadeAction.generated.h"

UENUM(BlueprintType)
enum class EEvadeDir : uint8
{
	FRONT, BACK, LEFT, RIGHT,
	MAX
};

/*
 * @brief 회피 동작을 정의하는 구조체
 */
USTRUCT(BlueprintType)
struct FEvadeData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Data")
	EEvadeDir Dir;
	
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	float PlayRate = 1.0f;

public:
	void PlayMontage(class ACharacter* InOwner);
};

//////////////////////////////////////////////////////////////////////////
/// IEvadeAction
////////////////////////////////////////////////////////////////////////
UINTERFACE(MinimalAPI)
class UIEvadeAction : public UInterface
{
	GENERATED_BODY()
};

/*
 * @brief 회피 인터페이스
 */
class CPROJECT_API IIEvadeAction
{
	GENERATED_BODY()

public:
	// 회피시 호출되는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Evade")
	void Evade();
	virtual void Evade_Implementation() {}
	// 회피 종료시 호출되는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Evade")
	void EndEvade();
	virtual void EndEvade_Implementation() {}

	// 회피 버튼을 눌렀을 때 호출되는 함수
	virtual void OnEvade() {}
};
