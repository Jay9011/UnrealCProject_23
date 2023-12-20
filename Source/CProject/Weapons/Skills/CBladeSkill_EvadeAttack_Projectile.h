#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkill.h"
#include "NiagaraCommon.h"
#include "Weapons/CWeaponStructures.h"
#include "DebugHeader.h"
#include "CBladeSkill_EvadeAttack_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
/**
 * @brief 원형 충돌체를 가진 발사체
 */
UCLASS(Blueprintable)
class CPROJECT_API ACBladeSkill_EvadeAttack_Projectile : public ACSkill
{
	GENERATED_BODY()

public:
	ACBladeSkill_EvadeAttack_Projectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void ActiveSkill() override;

private:
	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									   const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
									 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Lifetime = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FActionData ActionData;
	
private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* Niagara;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
};
