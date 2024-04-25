#pragma once

#include "CoreMinimal.h"
#include "AddOn/CAddOnComponent.h"
#include "CAddOn_StealthKill.generated.h"

class UCTakedownAreaComponent;
class ACBaseCharacter;
class UCCameraModifier;
/**
 * @brief Weapon에 부착되는 StealthKill AddOn
 */
UCLASS(Blueprintable)
class CPROJECT_API UCAddOn_StealthKill : public UCAddOnComponent
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnStealthKill(ACBaseCharacter* Target, UCTakedownAreaComponent* TakedownArea);
	UFUNCTION()
	void EndStealthKill();

private:
	UPROPERTY(EditAnywhere, Category = "Sequence")
	class ULevelSequence* StealthKillSequence = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	TSoftObjectPtr<AActor> PlayerLocationActor;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class UAnimMontage* StealthKillMontage = nullptr;

	UPROPERTY(EditAnywhere, Category = "Sequence")
	class UAnimMontage* TakedownMontage = nullptr;

private:
	UPROPERTY()
	UCCameraModifier* CameraModifier = nullptr;
};
