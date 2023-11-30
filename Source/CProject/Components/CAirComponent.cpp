#include "Components/CAirComponent.h"

#include "CMovementComponent.h"
#include "CStateComponent.h"
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
	
	// 만약 타이머가 실행되기 전이라면, 현재 중력은 복구되지 않았으므로, 복구되지 않은 중력을 저장하지 않는다.
	if (TimerManager.IsTimerActive(GravityRecoveryTimerHandle))
		OriginalGravityScale = MovementComponent->GravityScale;
	
	MovementComponent->GravityScale = InGravityScale;
	MovementComponent->Velocity = InVelocity;
	
	TimerManager.SetTimer(GravityRecoveryTimerHandle, TimerDelegate, InRecoveryTime, false);
}

void UCAirComponent::RecoveryGravityScale()
{
	CheckNull(OwnerCharacter);
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();

	CheckNull(MovementComponent);
	// 만약 타이머가 실행되기 전이라면, 타이머를 중지하고 중력을 바로 복구 시켜준다.
	FTimerManager& TimerManager = OwnerCharacter->GetWorldTimerManager();
	if (!TimerManager.IsTimerActive(GravityRecoveryTimerHandle))
	{
		TimerManager.ClearTimer(GravityRecoveryTimerHandle);
		MovementComponent->GravityScale = OriginalGravityScale;
		return;
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