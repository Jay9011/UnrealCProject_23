#pragma once

#include "CoreMinimal.h"
#include "CParryAction.h"
#include "CParryAction_Blade.generated.h"

class UCCameraRotationComponent;
class ACBaseCharacter;
/**
 * @brief Blade의 패링 성공시 발동하는 액션
 */
UCLASS(Blueprintable)
class CPROJECT_API UCParryAction_Blade : public UCParryAction
{
	GENERATED_BODY()
public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon) override;

public:
	virtual void End_DoAction() override;
	virtual void End_Action() override;

public:
	virtual void DoAction(ACharacter* InAttacker) override;
	
public:
	virtual void OnAttachmentEndCollision() override;
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	TArray<class ACharacter*> DamagedCharacters;
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* CameraRotationCurve;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float PlayRate = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotationMultiplier = 20.0f;

	UPROPERTY()
	UCCameraRotationComponent* CameraRotationComponent;

};
