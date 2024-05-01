#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPlayerInfoComponent.generated.h"


class UCUserWidget_PlayerInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCPlayerInfoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPlayerInfoComponent();

protected:
	virtual void BeginPlay() override;

private:
	void InitPlayerInfoWidget();
	void InitNoneClickableWidgets();

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Info")
	void SetEnemyInfo(const FText& InName, const class ACBaseCharacter* InTarget);

	UFUNCTION(BlueprintCallable, Category = "Enemy Info")
	void SetEnemyHp_Width(float InWidth);

	UFUNCTION(BlueprintCallable, Category = "Enemy Info")
	void ShowOrHideEnemyInfo(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void ShowOrHidePlayerInfo(bool bShow);
	
private:
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<UCUserWidget_PlayerInfo> PlayerInfoWidget;

	UPROPERTY(EditAnywhere, Category = "None Clickable")
	TArray<TSubclassOf<UUserWidget>> NoneClickableWidgets;

private:
	UPROPERTY()
	UCUserWidget_PlayerInfo* PlayerInfoWidgetInstance;
	
	TArray<UUserWidget*> NoneClickableWidgetsInstance;
};
