#include "Character/CAnimInstance.h"
#include "Global.h"

#include "Components/CNeckComponent.h"
#include "GameFramework/Character.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	// StateComponent를 찾아서 StateTypeChanged 델리게이트에 OnStateTypeChanged를 바인딩해 상태 변경을 감지한다.
	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	if(!!State)
	{
		// 상태 변경을 감지하면 OnStateTypeChanged 함수를 호출한다.
		State->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	}

	// WeaponComponent
	Weapon = Cast<UCWeaponComponent>(OwnerCharacter->GetComponentByClass(UCWeaponComponent::StaticClass()));
	if(!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
	}

	// MovementComponent
	Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	if(!!Movement)
	{
		Movement->OnStandingTypeChanged.AddDynamic(this, &UCAnimInstance::OnStandingTypeChanged);
	}

	// AirComponent
	Air = Cast<UCAirComponent>(OwnerCharacter->GetComponentByClass(UCAirComponent::StaticClass()));
	if(!!Air)
	{
		Air->OnAirStateChanged.AddDynamic(this, &UCAnimInstance::OnAirStateChanged);
	}

	// FeetComponent
	Feet = Cast<UCFeetComponent>(OwnerCharacter->GetComponentByClass(UCFeetComponent::StaticClass()));
	if (!!Feet)
	{
		UseFeetIK = true;
	}

	// NeckComponent
	Neck = Cast<UCNeckComponent>(OwnerCharacter->GetComponentByClass(UCNeckComponent::StaticClass()));
	if (!!Neck)
	{
		UseNeckRotation = true;
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	// Use Control Rotation을 사용하면 컨트롤러의 회전값을 사용하고, 아니면 캐릭터의 속도를 사용한다.
	if(OwnerCharacter->bUseControllerRotationYaw)
	{
		const FRotator Rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
		const FRotator Rotator2 = OwnerCharacter->GetControlRotation();
		const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(Rotator, Rotator2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, Delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;
	}

	// AirState가 Normal이 아니라면, 즉 공중에 있다면, Pitch를 0으로 초기화한다.
	{
		if(AirState != EAirState::Normal)
			Pitch = 0;
		else
			Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);
	}

	// Feet IK
	if (UseFeetIK)
	{
		FeetData = Feet->GetFeetData();
	}

	// Head Rotation
	if (UseNeckRotation)
	{
		NeckRotation = Neck->GetEffectorRotation();
	}
}

void UCAnimInstance::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::OnAirStateChanged(EAirState InPrevState, EAirState InNewState)
{
	AirState = InNewState;
	
	if (Air->IsAir())
		isAir = true;
	else
		isAir = false;
}

void UCAnimInstance::OnStandingTypeChanged(EStandingType InPrevType, EStandingType InNewType)
{
	StandingType = InNewType;
}

void UCAnimInstance::EnableNeckRotation()
{
	if (Neck != nullptr)
	{
		UseNeckRotation = true;		
	}
}

void UCAnimInstance::DisableNeckRotation()
{
	UseNeckRotation = false;
}
