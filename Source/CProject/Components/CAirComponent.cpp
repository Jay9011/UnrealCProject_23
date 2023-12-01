#include "Components/CAirComponent.h"

#include "CMovementComponent.h"
#include "GameFramework/Character.h"
#include "Utilities/CheckMacros.h"

UCAirComponent::UCAirComponent()
{
}

void UCAirComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	CheckNull(OwnerCharacter);
	OwnerCharacter->LandedDelegate.AddDynamic(this, &UCAirComponent::OnFLanded);
}

void UCAirComponent::SetGravityScale(float InGravityScale, FVector InVelocity, float InRecoveryTime)
{
	CheckNull(OwnerCharacter);
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();

	CheckNull(MovementComponent);

	FTimerManager& TimerManager = OwnerCharacter->GetWorldTimerManager();
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName("RecoveryGravityScale"));

	// 중력 적용
	MovementComponent->GravityScale = InGravityScale;
	MovementComponent->Velocity = InVelocity;
	
	TimerManager.SetTimer(GravityRecoveryTimerHandle, TimerDelegate, InRecoveryTime, false);
}

void UCAirComponent::RecoveryGravityScale()
{
	CheckNull(OwnerCharacter);
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();

	CheckNull(MovementComponent);
	MovementComponent->GravityScale = OriginalGravityScale;
	
	// 만약, 진행중인 타이머가 있다면 취소한다. (타이머 실행 전 함수가 호출되었을 경우)
	FTimerManager& TimerManager = OwnerCharacter->GetWorldTimerManager();
	GravityRecoveryTimerCancel(TimerManager);
}

void UCAirComponent::UpdateOriginalGravityScale()
{
	if (!!OwnerCharacter)
	{
		OriginalGravityScale = OwnerCharacter->GetCharacterMovement()->GravityScale;
	}
}

void UCAirComponent::GravityRecoveryTimerCancel(FTimerManager& InTimerManager)
{
	if (InTimerManager.IsTimerActive(GravityRecoveryTimerHandle))
	{
		InTimerManager.ClearTimer(GravityRecoveryTimerHandle);
	}
}

void UCAirComponent::ChangeAirState(EAirState InState)
{
	EAirState PrevState = AirState;
	AirState = InState;

	if (OnAirStateChanged.IsBound())
	{
		OnAirStateChanged.Broadcast(PrevState, AirState);		
	}
}

void UCAirComponent::OnFLanded(const FHitResult& Hit)
{
	SetLandMode();
}

void UCAirComponent::SetLandMode()
{
	// 이전 상태가 공중 상태였다면, 공중 상태의 종류에 따라 Standing Type을 변경.
	CheckTrue(AirState == EAirState::Normal);

	UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));

	if (!!MovementComponent)
	{
		// 적에 의해 날아가는 상태라면, 넉다운 상태로 변경.
		if (AirState == EAirState::Airborne)
		{
			MovementComponent->SetKnockDownMode();
		}
		else
			MovementComponent->SetStandingMode();		
	}

	
	ChangeAirState(EAirState::Normal);
}

void UCAirComponent::SetJumpMode()
{
	ChangeAirState(EAirState::Jump);
}

void UCAirComponent::SetFallMode()
{
	ChangeAirState(EAirState::Fall);
}

void UCAirComponent::SetAirborneMode()
{
	ChangeAirState(EAirState::Airborne);
}