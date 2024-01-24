#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Stone.generated.h"

class ACSkill_Stone;
/**
 *	@brief 발사체를 발사하는 DoAction 클래스
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoAction_Stone : public UCDoAction
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon) override;
	
public:
	virtual void DoAction_Implementation() override;

	virtual void Begin_Action() override;
	virtual void Begin_DoAction() override;
	virtual void End_Action() override;
	virtual void End_DoAction() override;

	virtual void OnEndEquip() override;
	virtual void OnUnEquip() override;

private:
	bool CreateStone();
	
private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<ACSkill_Stone> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	FName SocketName = "Hand_Stone";

	UPROPERTY()
	ACSkill_Stone* ReadyProjectile = nullptr;

private:
	TWeakObjectPtr<UCDoSubAction> SubAction = nullptr;

	FVector SavedDirection;

public:
	FORCEINLINE bool ReadyProjectileCheck() const;
	FORCEINLINE FVector GetSocketLocation() const;
	FORCEINLINE FVector GetDirection() const;
	FORCEINLINE float GetProjectileSpeed() const;
	FORCEINLINE float GetProjectileRadius() const;
};
