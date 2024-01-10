#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CMovementComponent.h"
#include "CAnimNotify_EndDown.generated.h"

/**
 * @brief 다운 상태를 해제하고 다른 상태로 변경
 */
UCLASS()
class CPROJECT_API UCAnimNotify_EndDown : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "ToType")
	EStandingType ToType;
};
