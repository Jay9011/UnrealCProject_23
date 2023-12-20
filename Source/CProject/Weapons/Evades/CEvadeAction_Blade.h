#pragma once

#include "CoreMinimal.h"
#include "Weapons/CEvadeAction.h"
#include "CEvadeAction_Blade.generated.h"

class ACGameMode;
class UCJustEvadeComponent;
/**
 * @brief Blade 회피 클래스
 */
UCLASS(Blueprintable)
class CPROJECT_API UCEvadeAction_Blade : public UCEvadeAction
{
private:
	GENERATED_BODY()

public:
	virtual void BeginPlay(ACharacter* InOwner, TArray<FEvadeData>& InData) override;
	
public:
	virtual void OnBeginEquip() override;
	virtual void OnUnequip() override;

public:
	virtual void BeginEvade() override;
	virtual void EndEvade() override;

private:
	void JustEvadeStart();
	void JustEvadeEnd();

public:
	bool CheckJustEvade();

/*
 * Just 회피 관련 멤버 변수
 */
private:
	UPROPERTY()
	ACGameMode* GameMode = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "JustEvade")
	float CheckRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "JustEvade")
	float Dilation = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "JustEvade")
	float Duration = 1.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	bool UseGhostTrail = true;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	float TrailInterval = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	float TrailFirstDelay = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	float TrailDuration = 1.f;
	
	UPROPERTY()
	UCJustEvadeComponent* JustEvadeComponent = nullptr;

	UPROPERTY()
	FTimerHandle JustEvadeTimerHandle;
};
