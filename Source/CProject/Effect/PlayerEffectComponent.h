#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerEffectComponent.generated.h"


class UCCameraArmLengthEffect;
class UDilationTimelineObject;
class USpringArmComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UPlayerEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerEffectComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void DilationEffect(ACharacter* InOwnerCharacter, const float InTargetDilation, TArray<AActor*>& InIgnoreActors);
	void ArmLengthEffect(USpringArmComponent* InSpringArm, const float InTargetArmLength);
	
private:
	UPROPERTY()
	UDilationTimelineObject* DilationTimelineObject = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Dilation")
	UCurveFloat* DilationCurve = nullptr;
	
	UPROPERTY()
	UCCameraArmLengthEffect* CameraArmLengthEffect = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "ArmLength")
	UCurveFloat* ArmLengthCurve = nullptr;
};
