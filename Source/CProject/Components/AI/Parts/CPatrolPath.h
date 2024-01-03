#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

class UTextRenderComponent;
class USplineComponent;

UCLASS()
class CPROJECT_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:
	ACPatrolPath();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
protected:
	virtual void BeginPlay() override;

public:
	FVector GetMoveTo();
	void UpdateIndex();

private:
	UPROPERTY(EditAnywhere, Category = "Path")
	int32 Index = 0;

	UPROPERTY(EditAnywhere, Category = "Path")
	bool bReverse = false;
	
	UPROPERTY(EditAnywhere, Category = "Path|Loop")
	bool bLoop = false;

private:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	USplineComponent* Spline = nullptr;
	
	UPROPERTY(VisibleDefaultsOnly)
	UTextRenderComponent* Text = nullptr;

	/*
	 * Getter
	 */
public:
	FORCEINLINE USplineComponent* GetSpline() const { return Spline; };
};
