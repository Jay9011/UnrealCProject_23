#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndAction.generated.h"

class UCWeaponComponent;
class IIExcuteAction;
/**
 * @brief DoAction을 끝내는 노티파이
 */
UCLASS()
class CPROJECT_API UCAnimNotify_EndAction : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UCWeaponComponent* weapon;
	IIExcuteAction* action;
};
