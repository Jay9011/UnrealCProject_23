#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PlayerInfo.generated.h"

class ACBaseCharacter;
/**
 * @brief Player의 정보를 표시하는 위젯
 */
UCLASS()
class CPROJECT_API UCUserWidget_PlayerInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Info")
	void SetEnemyInfo(const FText& InName, const ACBaseCharacter* InTarget);

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Info")
	void SetEnemyHp_Width(float InWidth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Info")
	void ShowEnemyInfo();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy Info")
	void HideEnemyInfo();
};
