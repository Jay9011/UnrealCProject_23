#include "Weapons/Projectiles/CPredictionPathEndPoint.h"

#include "Components/DecalComponent.h"

ACPredictionPathEndPoint::ACPredictionPathEndPoint()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Root);
}

void ACPredictionPathEndPoint::BeginPlay()
{
	Super::BeginPlay();
}
