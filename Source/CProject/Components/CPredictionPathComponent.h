#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "CPredictionPathComponent.generated.h"

class USplineMeshComponent;
class ACPredictionPathEndPoint;
class USplineComponent;
class UDecalComponent;

UCLASS( ClassGroup=("Projectile"), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCPredictionPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPredictionPathComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ShowAndUpdate(FPredictProjectilePathParams& Params);
	void Hide();

private:
	void CreateSplineMesh();
	
private:
	TWeakObjectPtr<USplineComponent> Spline = nullptr;

private:
	UPROPERTY(EditAnywhere, Category = "Path", DisplayName = "EndPoint")
	TSubclassOf<ACPredictionPathEndPoint> EndPointClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Path", DisplayName = "Mesh")
	TSubclassOf<USplineMeshComponent> SplineMeshClass = nullptr;

private:
	TArray<USplineMeshComponent*> PredictionPathMesh;
	
	UPROPERTY()
	ACPredictionPathEndPoint* EndPoint = nullptr;
};
