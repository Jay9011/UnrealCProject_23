#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CSequenceNotifyState.generated.h"

/**
 * @brief 시퀀스를 실행시키는 AnimNotifyState
 */
UCLASS()
class CPROJECT_API UCSequenceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Sequence")
	class UAnimSequenceBase* Sequence = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Sequence")
	float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	float BlendInTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	float BlendOutTime = 0.0f;

public:
	virtual FString GetNotifyName_Implementation() const override;
};
