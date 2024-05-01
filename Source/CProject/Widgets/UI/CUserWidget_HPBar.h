#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HPBar.generated.h"

/**
 *	@brief HP Bar 공통 위젯
 */
UCLASS()
class CPROJECT_API UCUserWidget_HPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetOwner(class ACBaseCharacter* InOwner);
	virtual void SetOwner_Implementation(class ACBaseCharacter* InOwner);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;
	
private:
	UPROPERTY()
	class ACBaseCharacter* Owner;
};
