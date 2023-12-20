#include "Character/EnemyType/CEnemy_HumanType.h"

#include "Character/CAnimInstance.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"

ACEnemy_HumanType::ACEnemy_HumanType()
{
	Movement = CreateDefaultSubobject<UCMovementComponent>("Movement");
	Weapon = CreateDefaultSubobject<UCWeaponComponent>("Weapon");

	TSubclassOf<UCAnimInstance> AnimInstance = ConstructorHelpers::FClassFinder<UCAnimInstance>(TEXT("AnimBlueprint'/Game/Character/ABP_BaseCharacter.ABP_BaseCharacter_C'")).Class;
	GetMesh()->SetAnimClass(AnimInstance);
}

void ACEnemy_HumanType::BeginPlay()
{
	Super::BeginPlay();
}
