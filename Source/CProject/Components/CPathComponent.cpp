#include "Components/CPathComponent.h"


#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#define SELF_DEBUG true

UCPathComponent::UCPathComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Path");
}

void UCPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// 미리 정의된 함수를 바인딩한다.
	FOnTimelineFloat OnTimelineCallback;
	OnTimelineCallback.BindUFunction(this, "SetTimelineProgress");
	const TWeakObjectPtr<UCurveFloat> CurveFloat = NewObject<UCurveFloat>(this, FName("CurveFloat"));
	MovementTimeline.AddInterpFloat(CurveFloat.Get(), OnTimelineCallback, FName("Default"), FName("Default"));

	FOnTimelineEvent OnTimelineFinishedCallback;
	OnTimelineFinishedCallback.BindUFunction(this, "OnTimelineFinished");
	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinishedCallback);
}

void UCPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MovementTimeline.TickTimeline(DeltaTime);
}

void UCPathComponent::MakePath(const FVector& InStart, const FVector& InEnd, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseMidPoint, const FVector& InMidPoint)
{
	SplineComponent->ClearSplinePoints(false);

	TArray<FVector> Points;

	if(bUseMidPoint)
	{
		Points = {InStart, InMidPoint, InEnd};
	}
	else
	{
		Points = {InStart, InEnd};
	}

	SplineComponent->SetSplinePoints(Points, CoordinateSpace, true);

	if(Points.Num() > 1)
		SplineComponent->SetRotationAtSplinePoint(0, UKismetMathLibrary::FindLookAtRotation(Points[0], Points[1]), CoordinateSpace, true);
}

void UCPathComponent::MoveAlongPath(float InSpeed, float InRate, UCurveFloat* InFloatCurve)
{
	Speed = InSpeed;
	
	if (InFloatCurve != nullptr)
	{
		MovementTimeline.SetFloatCurve(InFloatCurve, "Default");
	}

	MovementTimeline.SetPlayRate(InRate);
	MovementTimeline.PlayFromStart();
}

void UCPathComponent::SetTimelineProgress(float InValue)
{
	FVector ActorLocation = GetOwner()->GetActorLocation();
	FVector NextPoint = SplineComponent->GetLocationAtTime(InValue, ESplineCoordinateSpace::World);

	FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(ActorLocation, NextPoint);
	FVector Velocity = FVector::Distance(ActorLocation, NextPoint) * Speed * Direction;

	ACharacter* Owner = Cast<ACharacter>(GetOwner());
	Owner->GetMovementComponent()->Velocity = Velocity;
}

void UCPathComponent::OnTimelineFinished()
{
	ResetOriginalSpeed();
}
