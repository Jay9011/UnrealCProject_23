#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "CUserWidget_interact.generated.h"

class USizeBoxSlot;
class UTextBlock;
class UImage;
class UCanvasPanelSlot;
class USizeBox;
/**
 *	@brief 상호작용 위젯 
 */
UCLASS()
class CPROJECT_API UCUserWidget_interact : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta = (BindWidget))
	UImage* KeyImage = nullptr;
	
	UPROPERTY()
	USizeBox* KeyBox = nullptr;

	// UPROPERTY()
	// UCanvasPanelSlot* WidgetSlot = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text = nullptr;

public:
	FORCEINLINE FVector2D GetWidgetSize() const;
	FORCEINLINE void SetText(FText InText) const;
};
