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
	float Health = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;

private:
	class ACharacter* OwnerCharacter = nullptr;
	
/*
 * Getter
 */
public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; };

	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };

	UFUNCTION(BlueprintCallable, Category = "Status")
	FORCEINLINE bool IsDead() const { return Health <= 0.f; };
};