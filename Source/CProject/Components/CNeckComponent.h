#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyDebugger/IDebugCollector.h"
#include "DebugHeader.h"
#include "CNeckComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCNeckComponent : public UActorComponent
	, public IIDebugCollector
{
	GENERATED_BODY()

public:	
	UCNeckComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CheckNearLimitYaw(float InNearValue = 5.f);

private:
	FRotator GetHeadRotation();
	
private:
	UPROPERTY(EditAnywhere, Category = "Setting")
	float InterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float MinPitch = -30.f;
	
	UPROPERTY(EditAnywhere, Category = "Setting")
	float MaxPitch = 60.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float MinYaw = -50.f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float MaxYaw = 50.f;

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	FRotator EffectorRotation;
/*
 * Getter
 */
public:
	FRotator GetEffectorRotation() const { return EffectorRotation; }

	
#if DEBUG_HEAD
	virtual bool IsDebugEnable() override;
	virtual FDebugInfo GetDebugInfo() override;

private:
	FRotator DebugControlRotation;
	FRotator DebugActorRotation;
#endif
	
};
