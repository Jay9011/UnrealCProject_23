#include "Components/CMovementComponent.h"
#include "Utilities/CheckMacros.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
{

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnCrouchWalk()
{
	SetSpeed(ESpeedType::CrouchWalk);
}

void UCMovementComponent::OnCrouchRun()
{
	SetSpeed(ESpeedType::CrouchRun);
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::BackupControlRotation()
{
	bBackupRotationYaw = OwnerCharacter->bUseControllerRotationYaw;
	bBackupOrientRotationToMovement = OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement;
}

void UCMovementComponent::RestoreControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = bBackupRotationYaw;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = bBackupOrientRotationToMovement;
}


void UCMovementComponent::SetStandingMode()
{
	ChangeStandingType(EStandingType::Standing);
}

void UCMovementComponent::SetCrouchMode()
{
	ChangeStandingType(EStandingType::Crouch);
}

void UCMovementComponent::SetKnockDownMode()
{
	ChangeStandingType(EStandingType::KnockDown);
}

void UCMovementComponent::ChangeStandingType(EStandingType InType)
{
	EStandingType PrevType = StandingType;
	StandingType = InType;

	if (OnStandingTypeChanged.IsBound())
	{
		OnStandingTypeChanged.Broadcast(PrevType, StandingType);
	}
}