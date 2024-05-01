#include "Widgets/UI/CUserWidget_HPBar.h"

#include "Character/CBaseCharacter.h"
#include "Components/CStatusComponent.h"
#include "Utilities/CheckMacros.h"

void UCUserWidget_HPBar::SetOwner_Implementation(ACBaseCharacter* InOwner)
{
	Owner = InOwner;
}

float UCUserWidget_HPBar::GetHealthPercent() const
{
	if (Owner == nullptr)
		return 0.0f;

	UCStatusComponent* StatusComponent = Cast<UCStatusComponent>(Owner->GetComponentByClass(UCStatusComponent::StaticClass()));
	CheckNullResult(StatusComponent, 0.0f);

	return (StatusComponent->GetHealth() / StatusComponent->GetMaxHealth());
}
