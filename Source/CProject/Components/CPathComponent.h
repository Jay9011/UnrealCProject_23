#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "CPathComponent.generated.h"

#define SELF_DEBUG true

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPathComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void MakePath(const FVector& InStart, const FVector& InEnd, ESplineCoordinateSpace::Type CoordinateSpace = ESplineCoordinateSpace::World , bool bUseMidPoint = false, const FVector& InMidPoint = FVector::ZeroVector);
	void MoveAlongPath(float InSpeed = 10.f, float InRate = 1.f, UCurveFloat* InFloatCurve = nullptr);

private:
	UFUNCTION()
	void SetTimelineProgress(float InValue);

	UFUNCTION()
	void OnTimelineFinished();
	
private:
	TWeakObjectPtr<USplineComponent> SplineComponent;
	FTimeline MovementTimeline;

	float OriginalSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed = 10.f;

/*
 * Getter/Setter
 */
private:
	FORCEINLINE void ResetOriginalSpeed()
	{
#if SELF_DEBUG
		UE_LOG(LogTemp, Warning, TEXT("ResetOriginalSpeed: %f"), Speed);
#endif
		OriginalSpeed = Speed;
	}
};
