#pragma once

#include "CoreMinimal.h"
#include "Interface/FHitResultInterface.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoSubAction_Blade_AirToAirDash.generated.h"

/**
 * @brief 공중에서 공중으로 대쉬하는 SubAction
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_Blade_AirToAirDash : public UCDoSubAction
	, public IFHitResultInterface
{
	GENERATED_BODY()

private:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	virtual void Pressed() override;

	virtual void End_Action() override;

/*
 * 충돌체 관련 바운딩 함수
 */
public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision();
	UFUNCTION()
	virtual void OnAttachmentEndCollision();
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther);

/*
 * Path 관련 데이터
 */
private:
	FHitResult TargetHitResult;
	virtual void SetFHitResult(const FHitResult& InHitResult) override
	{
		TargetHitResult = InHitResult;
	}
	virtual const FHitResult& GetFHitResult() const override { return TargetHitResult; }
	
	TWeakObjectPtr<class UCPathComponent> OwnerPathComponent;

	UPROPERTY(EditAnywhere, Category = "Path")
	UCurveFloat* PathCurve;

	UPROPERTY(EditAnywhere, Category = "Path")
	float Rate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Path")
	float Speed = 10.f;
	
/*
 * Action 관련 데이터
 */
private:
	UPROPERTY(EditAnywhere, Category = "Action")
	FActionData ActionData;
	
/*
 * Action 관련 추가 데이터 
 */
private:
	UPROPERTY(EditDefaultsOnly, Category = "Air")
	float GravityScale = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Air")
	FVector Velocity = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, Category = "Air")
	float RecoveryTime = 0.5f;
};
