#include "Parts/CGhostTrail.h"

#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceConstant.h"

ACGhostTrail::ACGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	PoseableMeshComponent = CreateDefaultSubobject<UPoseableMeshComponent>("PoseableMesh");
	PoseableMeshComponent->SetupAttachment(RootComponent);
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	// GhostTrailMaterialInstance 초기화
	if(GhostTrailMaterialClass == nullptr)
	{
		UMaterialInstanceConstant* GhostTrailMaterial = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/Character/GhostTrail/MI_GhostTrail_Red.MI_GhostTrail_Red'")));

		if (GhostTrailMaterial != nullptr)
		{
			GhostTrailMaterialInstance = UMaterialInstanceDynamic::Create(GhostTrailMaterial, this);
		}
	}
	else
	{
		GhostTrailMaterialInstance = UMaterialInstanceDynamic::Create(GhostTrailMaterialClass, this);
	}

	// PoseableMeshComponent 초기화
	if (OwnerCharacter != nullptr)
	{
		// Local 트랜스폼을 OwnerCharacter와 일치시킨다.
		PoseableMeshComponent->SetRelativeTransform(OwnerCharacter->GetMesh()->GetRelativeTransform());
		PoseableMeshComponent->SetSkeletalMesh(OwnerCharacter->GetMesh()->SkeletalMesh);

		for (int32 i = 0; i < PoseableMeshComponent->SkeletalMesh->Materials.Num(); i++)
		{
			PoseableMeshComponent->SetMaterial(i, GhostTrailMaterialInstance);
		}
	}

	// 타임라인 초기화
	FOnTimelineFloat OpacityTimelineDelegate;
	OpacityTimelineDelegate.BindUFunction(this, "OpacityTimelineProgress");
	UCurveFloat* CurveFloat = NewObject<UCurveFloat>(this, FName("CurveFloat"));
	OpacityTimeline.AddInterpFloat(CurveFloat, OpacityTimelineDelegate, FName("Default"), FName("Default"));
	
	Show();
}

void ACGhostTrail::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	OpacityTimeline.TickTimeline(DeltaSeconds);
}

void ACGhostTrail::InitData(const FColor& InGhostTrailColor, const ACharacter* InOwner)
{
	if (OwnerCharacter == nullptr && InOwner != nullptr)
		OwnerCharacter = InOwner;
	
	GhostTrailColor = InGhostTrailColor;
}

bool ACGhostTrail::Start(const FTransform& InTransform)
{
	SetActorTransform(InTransform);
	Show();

	return true;
}

void ACGhostTrail::End()
{
	Hide();
	SetActorLocation(FVector::ZeroVector);
}

void ACGhostTrail::SetOpacity(UCurveFloat* InOpacityCurve, const float InDelayTime)
{
	
	if (FMath::IsNearlyZero(InDelayTime))
	{
		OpacityTimeline.SetFloatCurve(InOpacityCurve, "Default");
		OpacityTimeline.PlayFromStart();
	}
	else
	{
		FTimerHandle OpacityTimerHandle;
		FTimerDelegate OpacityTimerDelegate;
		OpacityTimerDelegate.BindLambda([this, InOpacityCurve]()
		{
			OpacityTimeline.SetFloatCurve(InOpacityCurve, "Default");
			OpacityTimeline.PlayFromStart();
		});
		GetWorld()->GetTimerManager().SetTimer(OpacityTimerHandle, OpacityTimerDelegate, InDelayTime, false);		
	}
}

void ACGhostTrail::Show()
{
	PoseableMeshComponent->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
	GhostTrailMaterialInstance->SetVectorParameterValue(ColorName, GhostTrailColor);
		
	PoseableMeshComponent->SetVisibility(true);
}

void ACGhostTrail::Hide()
{
	PoseableMeshComponent->SetVisibility(false);
}

void ACGhostTrail::OpacityTimelineProgress(float InValue)
{
	GhostTrailMaterialInstance->SetScalarParameterValue(OpacityName, InValue);
}

