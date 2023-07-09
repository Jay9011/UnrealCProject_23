#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CStateComponent.h"
#include "CAnimNotify_EndState.generated.h"

/**
 * @brief 애니메이션 종료 시 상태를 변경하는 애니메이션 노티파이 
 */
UCLASS()
class CPROJECT_API UCAnimNotify_EndState : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EStateType StateType;

public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
