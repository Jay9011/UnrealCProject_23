#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CStandUpAsset.generated.h"

UENUM(BlueprintType)
enum class EDir : uint8
{
	InPlace,
	FRONT, BACK, LEFT, RIGHT,
	MAX
};

/**
 * @brief 기상 동작과 관련된 데이터 에셋
 */
UCLASS()
class CPROJECT_API UCStandUpAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	void StandUp(EDir InDir, class ACharacter* InOwner);

private:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* StandUpMontage[(uint8)EDir::MAX];
};
