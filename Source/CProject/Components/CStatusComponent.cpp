#include "Components/CStatusComponent.h"
#include "DebugHeader.h"

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
	
#if DEBUG_STATUS
	UE_LOG(LogTemp, Log, TEXT("Health : %.2f / %.2f"), Health, MaxHealth);
#endif
}
