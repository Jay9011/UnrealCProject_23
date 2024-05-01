#include "Components/CPlayerInfoComponent.h"

#include "Blueprint/UserWidget.h"
#include "Widgets/CUserWidget_PlayerInfo.h"

UCPlayerInfoComponent::UCPlayerInfoComponent()
{
	
}


void UCPlayerInfoComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerInfoWidget();
	InitNoneClickableWidgets();
}

void UCPlayerInfoComponent::InitPlayerInfoWidget()
{
	if (PlayerInfoWidget)
	{
		PlayerInfoWidgetInstance = CreateWidget<UCUserWidget_PlayerInfo>(GetWorld(), PlayerInfoWidget);
		if (PlayerInfoWidgetInstance)
		{
			PlayerInfoWidgetInstance->AddToViewport();
			PlayerInfoWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
}

void UCPlayerInfoComponent::InitNoneClickableWidgets()
{
	for (auto WidgetClass : NoneClickableWidgets)
	{
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
			WidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
			NoneClickableWidgetsInstance.Add(WidgetInstance);
		}
	}
}

void UCPlayerInfoComponent::SetEnemyInfo(const FText& InName, const ACBaseCharacter* InTarget)
{
	if (PlayerInfoWidgetInstance)
	{
		PlayerInfoWidgetInstance->SetEnemyInfo(InName, InTarget);
	}
}

void UCPlayerInfoComponent::SetEnemyHp_Width(float InWidth)
{
	if (PlayerInfoWidgetInstance)
	{
		PlayerInfoWidgetInstance->SetEnemyHp_Width(InWidth);
	}
}

void UCPlayerInfoComponent::ShowOrHideEnemyInfo(bool bShow)
{
	if (PlayerInfoWidgetInstance)
	{
		if (bShow)
		{
			PlayerInfoWidgetInstance->ShowEnemyInfo();
		}
		else
		{
			PlayerInfoWidgetInstance->HideEnemyInfo();
		}
	}
}

void UCPlayerInfoComponent::ShowOrHidePlayerInfo(bool bShow)
{
	if (PlayerInfoWidgetInstance)
	{
		PlayerInfoWidgetInstance->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
	for (auto WidgetInstance : NoneClickableWidgetsInstance)
	{
		if (WidgetInstance)
		{
			WidgetInstance->SetVisibility(bShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
	}
}
