#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	void Damage(float InAmount);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Health")
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;

/*
 * Getter
 */
public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const { return MaxHealth; };

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool IsDead() const { return Health <= 0.f; };

	/// Setter
	UFUNCTION(BlueprintCallable, BlueprintSetter = "SetHealth", Category = "Health")
	void SetHealth(float InHealth)
	{
		Health = FMath::Clamp(InHealth, 0.f, MaxHealth);
	};

	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "Health")
	void SetMaxHealth(float InMaxHealth)
	{
		MaxHealth = FMath::Max(0.f, InMaxHealth);
	};

};