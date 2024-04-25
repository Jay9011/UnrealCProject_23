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

void UCInteractiveComponent::SetTarget(AActor* InTarget, ICInteractiveInterface* InInteractiveTarget)
{
	CheckNull(InInteractiveTarget);

	UObject* IInteractiveObject = Cast<UObject>(InInteractiveTarget);

	if (IInteractiveObject != nullptr)
	{
		InteractiveTarget = IInteractiveObject;
		Widget->SetText(InInteractiveTarget->GetInteractText());
		OnChangeTarget(InTarget);
	}
	
}

void UCInteractiveComponent::ClearTarget(ICInteractiveInterface* InInteractiveTarget)
{
	CheckNull(InInteractiveTarget);

	ICInteractiveInterface* IInteractiveInterface = Cast<ICInteractiveInterface>(InteractiveTarget.Get());

	if (IInteractiveInterface != nullptr && IInteractiveInterface == InInteractiveTarget)
	{
		InteractiveTarget.Reset();
		OnChangeTarget(nullptr);
	}
	else if (IInteractiveInterface == nullptr)
	{
		InteractiveTarget.Reset();
		OnChangeTarget(nullptr);
	}
}

void UCInteractiveComponent::CheckTarget()
{
	CheckNull(InteractiveTarget);

	ICInteractiveInterface* IInteractiveInterface = Cast<ICInteractiveInterface>(InteractiveTarget.Get());
	CheckNull(IInteractiveInterface);
	
	if (IInteractiveInterface->CanInteract(OwnerPlayer))
	{
		OnChangeTarget(IInteractiveInterface->GetInteractTarget());
	}
	else
	{
		OnChangeTarget(nullptr);
	}
}

void UCInteractiveComponent::OnInteract()
{
	CheckFalse(InteractiveTarget.IsValid());
	
	ICInteractiveInterface* IInteractiveInterface = Cast<ICInteractiveInterface>(InteractiveTarget.Get());
	CheckNull(IInteractiveInterface);

	if (WidgetTarget.IsValid() && IInteractiveInterface->CanInteract(OwnerPlayer))
	{
		IInteractiveInterface->OnInteract(OwnerPlayer);
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
