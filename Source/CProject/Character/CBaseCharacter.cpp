#include "Character/CBaseCharacter.h"

#include "AITypes.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACBaseCharacter::ACBaseCharacter()
{
	/*
	 * 컴포넌트 생성
	 */
	State = this->CreateDefaultSubobject<UCStateComponent>("State");
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
	MovementComponent->GravityScale = 2;
	MovementComponent->bUseSeparateBrakingFriction = true;

	
}