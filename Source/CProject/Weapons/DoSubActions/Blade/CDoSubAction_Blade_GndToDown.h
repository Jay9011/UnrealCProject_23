#pragma once

#include "CoreMinimal.h"
#include "Interface/FHitResultInterface.h"
#include "Weapons/CDoSubAction.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoSubAction_Blade_GndToDown.generated.h"

/**
 * @brief 땅에서 땅으로 내려찍는 SubAction
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_Blade_GndToDown : public UCDoSubAction
	, public IFHitResultInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	virtual void Pressed() override;

	virtual void End_Action() override;

/*
 * 충돌체 관련 바운딩 함수
 */
public:
	UFUNCTION()
	virtual void OnAttachmentEndCollision();
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, class UPrimitiveComponent* OverlappedComponent, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult);
	
public:
	virtual void SetFHitResult(const FHitResult& InHitResult) override
	{
		TargetHitResult = InHitResult;
	}
	virtual const FHitResult& GetFHitResult() const override { return TargetHitResult; }

private:
	FHitResult TargetHitResult;
	float MidHeight = 400.f;

	TWeakObjectPtr<class UCPathComponent> OwnerPathComponent;

	UPROPERTY(EditAnywhere, Category = "Path")
	UCurveFloat* PathCurve;

	UPROPERTY(EditAnywhere, Category = "Path")
	float Rate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Path")
	float Speed = 10.f;

/*
 * 공격형 액션 관리
 */
protected:
	UPROPERTY(EditAnywhere, Category = "Action")
	FActionData ActionData;
};
