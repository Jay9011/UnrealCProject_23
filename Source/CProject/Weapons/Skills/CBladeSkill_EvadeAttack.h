#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceExport.h"
#include "Weapons/CSkill.h"
#include "Weapons/CWeaponStructures.h"
#include "CBladeSkill_EvadeAttack.generated.h"

class ACPlayer;
class UPlayerEffectComponent;
class USphereComponent;

/**
 *	@brief 원형 폭발 스킬
 */
UCLASS(Blueprintable)
class CPROJECT_API ACBladeSkill_EvadeAttack : public ACSkill
{
	GENERATED_BODY()

public:
	ACBladeSkill_EvadeAttack();

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
	
/*
 * Niagara
 */
public:
	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* PSystem);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NiagaraComponent;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float TargetArmLength = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	float TargetDilation = 0.5f;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float SphereLifetime = 0.5f;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FActionData ActionData;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Add_on")
	TSubclassOf<class ACSkill> AddOnSkillClass;
	UPROPERTY(EditDefaultsOnly, Category = "Add_on")
	float AddOnSkillDelay = 0.3f;
	
private:
	UPROPERTY()
	ACPlayer* OwnerPlayer = nullptr;
	
	UPROPERTY()
	UPlayerEffectComponent* PlayerEffectComponent = nullptr;

	UPROPERTY()
	FTimerHandle AddOnSkillDelayTimerHandle;
};
