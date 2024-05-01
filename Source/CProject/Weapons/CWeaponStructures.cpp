#include "Weapons/CWeaponStructures.h"

#include "Character/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CheckMacros.h"
#include "Utilities/CHelpers.h"
#include "Utilities/GTimeController.h"

/* ========================================================================================================================================
 * FHitData
 * ========================================================================================================================================
 */
void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, UPrimitiveComponent* OverlappedComponent)
{
	FActionDamageEvent DamageEvent;
	DamageEvent.HitData = this;
	DamageEvent.OverlappedComponent = OverlappedComponent;
	
	InOther->TakeDamage(Damage, DamageEvent, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayMontage(ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!MovementComponent)
		bCanMove ? MovementComponent->Move() : MovementComponent->Stop();
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(HitStopTime));

	UGTimeController::SetTimeDilationOnlyActors(InWorld, 1e-3f, HitStopTime);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* World = InOwner->GetWorld();
	FVector Location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location);
}

void FHitData::PlayShake(AController* InOwner)
{
	CheckNull(Shake);

	APlayerController* PlayerController = Cast<APlayerController>(InOwner);
	CheckNull(PlayerController);

	PlayerController->PlayerCameraManager->StartCameraShake(Shake, 1.f);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform Transform;
	Transform.SetLocation(EffectLocation);
	Transform.SetScale3D(EffectScale);
	Transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, Transform);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform Transform;
	Transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	Transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, Transform);
}

/* ========================================================================================================================================
 * FDoActionData
 * ========================================================================================================================================
 */
void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (MovementLaunch != FVector::ZeroVector)
	{
		InOwner->LaunchCharacter(InOwner->GetActorRotation().RotateVector(MovementLaunch), true, true);
	}
	
	if (!!MovementComponent)
	{
		if(!bCanMove)
		{
			MovementComponent->Stop();
		}
		
		if(bFixedCamera)
		{
			MovementComponent->EnableFixedCamera();
		}
	}// if (!!MovementComponent)

	if(!!ActionMontage)
	{
		InOwner->PlayAnimMontage(ActionMontage, PlayRate);
	}// if(!!ActionMontage)

	if(!!Effect)
	{
		FTransform Transform;
		Transform.SetLocation(EffectLocation);
		Transform.SetScale3D(EffectScale);
		Transform.AddToTranslation(InOwner->GetActorLocation());

		CHelpers::PlayEffect(InOwner->GetWorld(), Effect, Transform);
	}// if(!!Effect)
}

/* ========================================================================================================================================
 * FActionData
 * ========================================================================================================================================
 */
void FActionData::InitActionData()
{
	if (ActionDataTable != nullptr)
	{
		TArray<FDoActionData*> ActionDataArray;
		ActionDataTable->GetAllRows<FDoActionData>("", ActionDataArray);

		for (FDoActionData* Data : ActionDataArray)
			ActionData.Add(*Data);
	}
	else if (ActionData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("ActionDataTable is nullptr"));
	}
}
