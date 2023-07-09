#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEvadeAction.generated.h"

UENUM(BlueprintType)
enum class EEvdeDir : uint8
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
	EEvdeDir Dir;
	
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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Evade")
	void Evade();
	virtual void Evade_Implementation() {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Evade")
	void EndEvade();
	virtual void EndEvade_Implementation() {}

	virtual void OnEvade() {}
};
