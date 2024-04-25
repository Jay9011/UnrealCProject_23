#include "AddOn/Weapon/CAddOn_StealthKill.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Character/CPlayer.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utilities/CheckMacros.h"
#include "Components/CapsuleComponent.h"
#include "Components/Interactive/CTakedownAreaComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCAddOn_StealthKill::OnStealthKill(ACBaseCharacter* Target, UCTakedownAreaComponent* TakedownArea)
{
	ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(Player);

	UCStateComponent* StateComponent = Player->GetStateComponent();
	CheckNull(StateComponent);

	StateComponent->SetActionMode();

	UCMovementComponent* Movement = Cast<UCMovementComponent>(Player->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(Movement);

	Movement->BackupControlRotation();
	Movement->DisableControlRotation();
	Movement->Stop();

	/*
	 * Actor Setting
	 */
	Player->SetActorEnableCollision(false);
	Target->SetActorEnableCollision(false);

	FVector PlayerNewLocation = Target->GetActorLocation() - Target->GetActorForwardVector() * 45.f;
	PlayerNewLocation.Z = Player->GetActorLocation().Z;
	Player->SetActorLocation(PlayerNewLocation);
	
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Player->GetActorLocation(), Target->GetActorLocation());
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;
	Player->SetActorRotation(NewRotation);
	
	/*
	 * Camera Animation
	 */
	CheckNull(StealthKillSequence);

	APlayerController* PlayerController = Player->GetController<APlayerController>();
	CheckNull(PlayerController);

	if (PlayerLocationActor.IsValid())
	{
		PlayerLocationActor->SetActorTransform(Player->GetTransform());
	}
	
	ALevelSequenceActor* SequenceActor = nullptr;
	UMovieSceneSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(PlayerController, StealthKillSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	SequencePlayer->Play();
	
	SequencePlayer->OnStop.AddDynamic(this, &UCAddOn_StealthKill::EndStealthKill);

	/*
	 * Play Animation
	 */
	OwnerCharacter->PlayAnimMontage(StealthKillMontage);
	TakedownArea->OnTakedownMontage(TakedownMontage);
}

void UCAddOn_StealthKill::EndStealthKill()
{
	ACPlayer* Player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(Player);
	
	UCStateComponent* StateComponent = Player->GetStateComponent();
	CheckNull(StateComponent);

	StateComponent->SetIdleMode();

	UCMovementComponent* Movement = Cast<UCMovementComponent>(Player->GetComponentByClass(UCMovementComponent::StaticClass()));
	CheckNull(Movement);

	Movement->RestoreControlRotation();
	Movement->Move();

	Player->SetActorEnableCollision(true);
}
