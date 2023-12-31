#include "Character/CBaseCharacter.h"

#include "AITypes.h"
#include "Components/CAirComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/UDirectionalUtilities.h"
#include "Weapons/CWeaponStructures.h"

ACBaseCharacter::ACBaseCharacter()
{
	/*
	 * 컴포넌트 생성
	 */
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	Montages = CreateDefaultSubobject<UCMontagesComponent>("Montages");

	/*
	 * 플레이어 위치 및 회전 설정
	 */
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetMesh()->SetSkeletalMesh(ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Resources/Mesh/SK_Mannequin.SK_Mannequin'")).Object);
	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACBaseCharacter::OnStateTypeChanged);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bUseSeparateBrakingFriction = true;
	MovementComponent->GravityScale = 2;
	
	UCAirComponent* AirComponent = Cast<UCAirComponent>(GetComponentByClass(UCAirComponent::StaticClass()));
	if (!!AirComponent)
	{
		AirComponent->UpdateOriginalGravityScale();
	}
}

void ACBaseCharacter::ApplyDamage()
{
	// 데미지 처리
	if (!!Status)
	{
		Status->Damage(Damaged.Damage);
	}
	Damaged.Damage = 0;
}

void ACBaseCharacter::LaunchAttacker(const FRotator& InLookAtRotation)
{
	FHitData* HitData = Damaged.Event->HitData;
	
	// 히트시 공격자에게 가해지는 효과
	if (HitData->AttackerLaunch != FVector::ZeroVector)
	{
		FVector RotatedAttackerLaunch = InLookAtRotation.RotateVector(HitData->AttackerLaunch);
		Damaged.Character->LaunchCharacter({RotatedAttackerLaunch.X, RotatedAttackerLaunch.Y, RotatedAttackerLaunch.Z}, true, true);
	}
}

void ACBaseCharacter::ResetDamagedData(FDamagedData& DamagedData)
{
	Damaged.Damage = 0;
	Damaged.Character = nullptr;
	Damaged.Causer = nullptr;
	Damaged.Event = nullptr;
}

FRotator ACBaseCharacter::LookAt(const ACharacter* InTarget)
{
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), InTarget->GetActorLocation());
	Rotator.Pitch = 0;
	Rotator.Roll = 0;
	SetActorRotation(Rotator);

	return Rotator;
}
