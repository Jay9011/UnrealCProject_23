#include "Components/CPredictionPathComponent.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Projectiles/CPredictionPathEndPoint.h"

UCPredictionPathComponent::UCPredictionPathComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void UCPredictionPathComponent::BeginPlay()
{
	Super::BeginPlay();

	if (EndPointClass != nullptr)
	{
		EndPoint = GetWorld()->SpawnActor<ACPredictionPathEndPoint>(EndPointClass);
		if (EndPoint != nullptr)
		{
			EndPoint->SetActorHiddenInGame(true);
		}
	}
}

void UCPredictionPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCPredictionPathComponent::ShowAndUpdate(FPredictProjectilePathParams& Params)
{
	FPredictProjectilePathResult Result;
	const bool IsHit = UGameplayStatics::PredictProjectilePath(GetOwner()->GetWorld(), Params, Result);

	// 시뮬레이션 결과 Hit 여부에 따라 EndPoint 세팅
	if (IsHit)
	{
		EndPoint->SetActorHiddenInGame(false);
		EndPoint->SetActorLocation(Result.HitResult.Location);
		EndPoint->SetActorRotation(Result.HitResult.ImpactNormal.Rotation());
	}
	else
	{
		EndPoint->SetActorHiddenInGame(true);
	}

	// Spline 세팅
	Spline->ClearSplinePoints();
	for (int32 i = 0; i < Result.PathData.Num(); ++i)
	{
		const FPredictProjectilePathPointData& PointData = Result.PathData[i];
		Spline->AddSplinePoint(PointData.Location, ESplineCoordinateSpace::World);
		Spline->SetSplinePointType(i, ESplinePointType::CurveClamped);

		if (PredictionPathMesh.IsValidIndex(i) == false)
		{
			CreateSplineMesh();
		}
	}

	// SplineMesh 세팅
	for (int32 i = 0; i < PredictionPathMesh.Num(); ++i)
	{
		if (i >= Result.PathData.Num() - 1)
		{
			PredictionPathMesh[i]->SetVisibility(false);
			continue;
		}

		FVector StartLocation, StartTangent, EndLocation, EndTangent;
		Spline->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::World);
		Spline->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::World);

		PredictionPathMesh[i]->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
		PredictionPathMesh[i]->SetVisibility(true);
	}
}

void UCPredictionPathComponent::Hide()
{
	if (EndPoint != nullptr)
	{
		EndPoint->SetActorHiddenInGame(true);
	}

	for (USplineMeshComponent* SplineMesh : PredictionPathMesh)
	{
		SplineMesh->SetVisibility(false);
	}
}

void UCPredictionPathComponent::CreateSplineMesh()
{
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, SplineMeshClass);
	if (SplineMesh)
	{
		SplineMesh->RegisterComponent();
		SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);

		PredictionPathMesh.Add(SplineMesh);
	}
}

