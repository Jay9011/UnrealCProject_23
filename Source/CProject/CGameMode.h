#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

class UCBattleSystemComponent;

UCLASS()
class CPROJECT_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();
	virtual void StartPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TEnumAsByte<ETraceTypeQuery> EvadableTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "Battle")
	TSubclassOf<UCBattleSystemComponent> BattleSystemComponent = nullptr;

	UPROPERTY()
	UCBattleSystemComponent* BattleSystem = nullptr;
	
/*
 * Getter
 */
public:
	FORCEINLINE TEnumAsByte<ETraceTypeQuery> GetEvadableType() const { return EvadableTraceChannel; }
	FORCEINLINE UCBattleSystemComponent* GetBattleSystemComponent() const { return BattleSystem; }

public:
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FORCEINLINE bool IsDebugBtn() const { return bDebugBtn; }
	UFUNCTION(BlueprintCallable, BlueprintSetter)
	void SetDebugBtn(bool bInDebugBtn) { bDebugBtn = bInDebugBtn; }
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bDebugBtn = false;
};
