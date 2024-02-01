#include "Components/CMovementComponent.h"

#include "Character/CPlayer.h"
#include "Utilities/CheckMacros.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyDebugger/DebuggerComponent.h"

UCMovementComponent::UCMovementComponent()
{

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (ControlRotation)
		EnableControlRotation();
	else
		DisableControlRotation();

	SetSpeed(InitSpeedType);

#if DEBUG_MOVEMENT
	if (UDebuggerComponent* DebugerComponent = Cast<UDebuggerComponent>(OwnerCharacter->GetComponentByClass(UDebuggerComponent::StaticClass())))
	{
		DebugerComponent->AddCollector(this);
	}
#endif
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
	if (OwnerCharacter == nullptr)
	{
		ControlRotation = true;
		return;
	}
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	if (OwnerCharacter == nullptr)
	{
		ControlRotation = false;
		return;
	}
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
	bBackupControlRotation = true;
}

void UCMovementComponent::RestoreControlRotation()
{
	if (!bBackupControlRotation)
	{
		return;
	}
	
	OwnerCharacter->bUseControllerRotationYaw = bBackupRotationYaw;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = bBackupOrientRotationToMovement;
	bBackupControlRotation = false;
}


void UCMovementComponent::SetStandingMode()
{
	OwnerCharacter->UnCrouch();
	ChangeStandingType(EStandingType::Standing);
}

void UCMovementComponent::SetCrouchMode()
{
	OwnerCharacter->Crouch();
	ChangeStandingType(EStandingType::Crouch);
}

void UCMovementComponent::SetKnockDownMode()
{
	ChangeStandingType(EStandingType::KnockDown);
}

void UCMovementComponent::SwitchCrouchMode()
{
	if (IsCrouch())
	{
		SetStandingMode();
	}
	else
	{
		SetCrouchMode();		
	}
}

void UCMovementComponent::OnCrouch()
{
	if (StandingType == EStandingType::Standing)
	{
		SetCrouchMode();
	}
}

void UCMovementComponent::OffCrouch()
{
	if (StandingType == EStandingType::Crouch)
	{
		SetStandingMode();
	}
}

void UCMovementComponent::StandUp(EDir InDir)
{
	CheckNull(StandUpAsset);

	bStandingProcess = true;
	StandUpAsset->StandUp(InDir, OwnerCharacter.Get());
}

void UCMovementComponent::ChangeStandingType(EStandingType InType)
{
	EStandingType PrevType = StandingType;
	StandingType = InType;
	
	if (bStandingProcess)
	{
		bStandingProcess = false;
	}

	if (OnStandingTypeChanged.IsBound())
	{
		OnStandingTypeChanged.Broadcast(PrevType, StandingType);
	}
}

void UCMovementComponent::Move()
{
	bCanMove = true;
}

void UCMovementComponent::Stop()
{
	bCanMove = false;
}

#if DEBUG_MOVEMENT
bool UCMovementComponent::IsDebugEnable()
{
	return true;
}

FDebugInfo UCMovementComponent::GetDebugInfo()
{
	FDebugInfo Info;
	Info.Priority = static_cast<int>(DEBUG_NUMS::MOVEMENT);
	Info.Data.Add({"Standing Type: " + StaticEnum<EStandingType>()->GetNameStringByValue(static_cast<int64>(StandingType)), FColor::Red});
	Info.Data.Add({"Speed: " + FString::SanitizeFloat(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed), FColor::Cyan});
	Info.Data.Add({"Can Move: " + (bCanMove ? FString("True") : FString("False")), FColor::Cyan});
	Info.Data.Add({"Use Control Rotation: " + (OwnerCharacter->bUseControllerRotationYaw ? FString("True") : FString("False")), FColor::Cyan});
	Info.Data.Add({"Fixed Camera: " + (bFixedCamera ? FString("True") : FString("False")), FColor::Cyan});

	return Info;
}
#endif