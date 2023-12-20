#pragma once

#include "CoreMinimal.h"
#include "Components/CAirComponent.h"
#include "Weapons/CDoSubAction.h"
#include "CDoSubAction_Blade.generated.h"

/**
 * @brief blade SubAction 선택기
 */
UCLASS(Blueprintable)
class CPROJECT_API UCDoSubAction_Blade : public UCDoSubAction
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(ACharacter* InOwner, UCWeaponObject* InWeapon, ACAttachment* InAttachment, UCDoAction* InDoAction) override;

public:
	virtual void Pressed() override;
	virtual void Released() override;

private:
	void PerformAerialStanceAction();
	void PerformGroundStanceAction();

	bool TryJustEvade();

private:
	bool TraceForwardNearDownEnemyByProfile(float InLength, float InRadius, const FName& InProfile, const TArray<AActor*>& InIgnoreActors, FHitResult& OutHitResult) const;

private:
	TWeakObjectPtr<UCAirComponent> AirComponent;

	UPROPERTY(EditAnywhere, Category = "SubAction")
	TSubclassOf<UCDoSubAction> NormalGroundActionClass;
	UPROPERTY(EditAnywhere, Category = "SubAction")
	TSubclassOf<UCDoSubAction> GroundToDownActionClass;
	UPROPERTY(EditAnywhere, Category = "SubAction")
	TSubclassOf<UCDoSubAction> AirToAirActionClass;
	UPROPERTY(EditAnywhere, Category = "SubAction")
	TSubclassOf<UCDoSubAction> AirToGroundActionClass;
	UPROPERTY(EditAnywhere, Category = "SubAction")
	TSubclassOf<UCDoSubAction> EvadeActionClass;

	UPROPERTY()
	UCDoSubAction* NormalGroundAction;
	UPROPERTY()
	UCDoSubAction* GroundToDownAction;
	UPROPERTY()
	UCDoSubAction* AirToAirAction;
	UPROPERTY()
	UCDoSubAction* AirToGroundAction;
	UPROPERTY()
	UCDoSubAction* EvadeAction;
	

private:
	UPROPERTY(EditAnywhere, Category = "AirTrace")
	float AirTraceLength = 1500.f;
	UPROPERTY(EditAnywhere, Category = "AirTrace")
	float AirTraceRadius = 100.f;
	UPROPERTY(EditAnywhere, Category = "AirTrace")
	FName AirTraceProfile = FName("Targeting");

	UPROPERTY(EditAnywhere, Category = "GroundTrace")
	float GroundTraceLength = 1500.f;
	UPROPERTY(EditAnywhere, Category = "GroundTrace")
	float GroundTraceRadius = 100.f;
	UPROPERTY(EditAnywhere, Category = "GroundTrace")
	FName GroundTraceProfile = FName("Targeting");
};
