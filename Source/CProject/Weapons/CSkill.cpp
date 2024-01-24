#include "Weapons/CSkill.h"

#include "CWeaponObject.h"
#include "GameFramework/Character.h"

ACSkill::ACSkill()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void ACSkill::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}
