#include "Components/AI/Parts/CPatrolPath.h"

#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	// Drag하는 동안에는 OnConstruction을 실행하지 않는다.
	bRunConstructionScriptOnDrag = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(Root);

	Text = CreateDefaultSubobject<UTextRenderComponent>("Text");
	Text->SetupAttachment(Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30));

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
#if WITH_EDITOR
	Text->Text = FText::FromString(GetActorLabel());
#endif

	Spline->SetClosedLoop(bLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 Count = Spline->GetNumberOfSplinePoints();

	// 역으로 이동하는 경우
	if (bReverse)
	{
		// 첫번째까지 이동하지 않은 경우
		if (Index > 0)
		{
			Index--;

			return;
		}

		// 역방향 이동시 순환되는 경우
		if (Spline->IsClosedLoop())
		{
			Index = Count - 1;

			return;
		}

		// 역방향 이동시 순환되지 않는 경우
		Index = 1;
		bReverse = false;

		return;
	}

	// 정방향으로 이동하는 경우
	// 마지막까지 이동하지 않은 경우
	if (Index < Count - 1)
	{
		Index++;

		return;
	}

	// 정방향 이동시 순환되는 경우
	if (Spline->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	// 정방향 이동시 순환되지 않는 경우
	Index = Count - 2;
	bReverse = true;
}
