#include "Weapons/CWeaponStructures.h"

#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CheckMacros.h"
#include "Utilities/GTimeController.h"

/* ========================================================================================================================================
 * FDoActionData
 * ========================================================================================================================================
 */
void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!MovementComponent)
	{
		if(bFixedCamera)
		{
			MovementComponent->EnableFixedCamera();
		}

		if(!bCanMove)
		{
			MovementComponent->Stop();
		}
	}// if (!!MovementComponent)

	if(!!ActionMontage)
	{
		InOwner->PlayAnimMontage(ActionMontage, PlayRate);
	}// if(!!ActionMontage)
}

/* ========================================================================================================================================
 * FHitData
 * ========================================================================================================================================
 */
void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent DamageEvent;
	DamageEvent.HitData = this;
	
	InOther->TakeDamage(20, DamageEvent, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!MovementComponent)
		bCanMove ? MovementComponent->Move() : MovementComponent->Stop();

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	UGTimeController::SetTimeDilationOnlyActors(InWorld, 1e-3f, StopTime);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* World = InOwner->GetWorld();
	FVector Location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location);
}
