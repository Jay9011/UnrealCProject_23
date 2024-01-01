#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

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

/*
 * Getter
 */
public:
	FORCEINLINE TEnumAsByte<ETraceTypeQuery> GetEvadableType() const { return EvadableTraceChannel; }

public:
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	FORCEINLINE bool IsDebugBtn() const { return bDebugBtn; }
	UFUNCTION(BlueprintCallable, BlueprintSetter)
	void SetDebugBtn(bool bInDebugBtn) { bDebugBtn = bInDebugBtn; }
	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bDebugBtn = false;
};
