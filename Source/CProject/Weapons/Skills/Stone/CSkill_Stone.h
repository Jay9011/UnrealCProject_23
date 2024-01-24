#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkill.h"
#include "CSkill_Stone.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
/**
 * @brief 돌
 */
UCLASS(Blueprintable)
class CPROJECT_API ACSkill_Stone : public ACSkill
{
	GENERATED_BODY()
public:
	ACSkill_Stone();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void ActiveSkill() override;
	void ActiveSkill(FVector& InDirection);

private:
	bool OnSimulatePhysics();
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StoneMesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	float LifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "Default")
	USoundBase* HitSound = nullptr;
	
private:
	bool bPlayedHitSound = false;
	bool bPhysicsOn = false;

public:
	FORCEINLINE float GetProjectileSpeed() const;
	FORCEINLINE float GetSphereRadius() const;
};
