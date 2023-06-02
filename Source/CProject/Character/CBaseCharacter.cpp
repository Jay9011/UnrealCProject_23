#include "Character/CBaseCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

ACBaseCharacter::ACBaseCharacter()
{
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	GetMesh()->SetSkeletalMesh(ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Resources/Mesh/SK_Mannequin.SK_Mannequin'")).Object);
	
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

