#include "Components/Interactive/CInteractiveComponent.h"

#include "Character/CPlayer.h"
#include "Utilities/CheckMacros.h"
#include "Components/Interactive/CUserWidget_interact.h"

UCInteractiveComponent::UCInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WidgetClass = ConstructorHelpers::FClassFinder<UCUserWidget_interact>(TEXT("WidgetBlueprint'/Game/Widgets/WB_Interactive_Key.WB_Interactive_Key_C'")).Class;
}

void UCInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<ACPlayer>(GetOwner());
	CheckNull(OwnerPlayer);

	OwnerPC = OwnerPlayer->GetController<APlayerController>();

	if (WidgetClass != nullptr)
	{
		Widget = CreateWidget<UCUserWidget_interact, APlayerController>(OwnerPC, WidgetClass);
		CheckNull(Widget);
		
		Widget->AddToViewport();
		Widget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClass is nullptr"));
	}
}

void UCInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (WidgetTarget.IsValid() && Widget != nullptr)
	{
		FVector2D ScreenLocation;
		if (OwnerPC && OwnerPC->ProjectWorldLocationToScreen(WidgetTarget->GetActorLocation(), ScreenLocation))
		{
			FVector2D WidgetSize = Widget->GetWidgetSize();
			ScreenLocation -= (WidgetSize * 0.5f);
			
			Widget->SetPositionInViewport(ScreenLocation, true);
		}
	}
}

void UCInteractiveComponent::SetTarget(AActor* InTarget, UObject* InInteractiveTarget)
{
	CheckNull(InInteractiveTarget);
	CheckFalse(InInteractiveTarget->GetClass()->ImplementsInterface(UCInteractiveInterface::StaticClass()));
	
	InteractiveTarget = InInteractiveTarget;
	Widget->SetText(ICInteractiveInterface::Execute_GetInteractText(InInteractiveTarget));
	
	CheckNull(InTarget);
	OnChangeTarget(InTarget);
}

void UCInteractiveComponent::ClearTarget(UObject* InInteractiveTarget)
{
	CheckNull(InInteractiveTarget);
	CheckFalse(InInteractiveTarget->GetClass()->ImplementsInterface(UCInteractiveInterface::StaticClass()));

	if (InteractiveTarget.Get() == InInteractiveTarget)
	{
		InteractiveTarget.Reset();
		OnChangeTarget(nullptr);
	}
	else if (!InteractiveTarget.IsValid())
	{
		InteractiveTarget.Reset();
		OnChangeTarget(nullptr);
	}
}

void UCInteractiveComponent::CheckTarget()
{
	CheckNull(InteractiveTarget);
	CheckFalse(InteractiveTarget->GetClass()->ImplementsInterface(UCInteractiveInterface::StaticClass()));
	
	if (ICInteractiveInterface::Execute_CanInteract(InteractiveTarget.Get(), OwnerPlayer))
	{
		OnChangeTarget(ICInteractiveInterface::Execute_GetInteractTarget(InteractiveTarget.Get()));
	}
	else
	{
		OnChangeTarget(nullptr);
	}
}

void UCInteractiveComponent::OnInteract()
{
	CheckFalse(InteractiveTarget.IsValid());
	CheckFalse(InteractiveTarget->GetClass()->ImplementsInterface(UCInteractiveInterface::StaticClass()));
	
	if (WidgetTarget.IsValid() && ICInteractiveInterface::Execute_CanInteract(InteractiveTarget.Get(), OwnerPlayer))
	{
		ICInteractiveInterface::Execute_OnInteract(InteractiveTarget.Get(), OwnerPlayer);
	}
}

void UCInteractiveComponent::OnChangeTarget(AActor* InTarget)
{
	CheckNull(Widget);

	if (InTarget)
	{
		WidgetTarget = InTarget;		
	}
	else
	{
		WidgetTarget.Reset();
	}
	
	Widget->SetVisibility(WidgetTarget != nullptr ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

ICInteractiveInterface* UCInteractiveComponent::GetInteractiveTarget() const
{
	return Cast<ICInteractiveInterface>(InteractiveTarget.Get());
}
