#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPredictionPathEndPoint.generated.h"

UCLASS(Blueprintable)
class CPROJECT_API ACPredictionPathEndPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPredictionPathEndPoint();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Root = nullptr;

	UPROPERTY(VisibleDefaultsOnly)
	UDecalComponent* Decal = nullptr;
	
};