#include "Components/Interactive/CUserWidget_interact.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Utilities/CheckMacros.h"

void UCUserWidget_interact::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget());
	// CheckNull(CanvasPanel);
	//
	// TArray<UWidget*> Children = CanvasPanel->GetAllChildren();
	// for (UWidget* Widget : Children)
	// {
	// 	USizeBox* SizeBox = Cast<USizeBox>(Widget);
	// 	if (SizeBox != nullptr)
	// 	{
	// 		KeyBox = SizeBox;
	// 		WidgetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(KeyBox);
	// 		break;
	// 	}
	// }

	// Key Image Setting
	CheckNull(KeyImage);

	KeyBox = Cast<USizeBox>(KeyImage->GetParent());
}

FVector2D UCUserWidget_interact::GetWidgetSize() const
{
	// return WidgetSlot->GetSize();
	return KeyBox->GetDesiredSize();
}

void UCUserWidget_interact::SetText(FText InText) const
{
	CheckNull(Text);

	Text->SetText(InText);
}
