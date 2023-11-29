#include "Components/CStatusComponent.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// 체력 초기화
	Health = MaxHealth;
}

void UCStatusComponent::Damage(float InAmount)
{
	Health += (InAmount * -1.f);
	Health = FMath::Clamp(Health, 0.f, MaxHealth);
}
