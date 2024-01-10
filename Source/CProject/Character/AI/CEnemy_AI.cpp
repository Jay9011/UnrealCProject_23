#include "Character/AI/CEnemy_AI.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Components/CNeckComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/AI/CAIBehaviorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CheckMacros.h"
#include "Widgets/CUserWidget_Label.h"

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	BehaviorComponent = CreateDefaultSubobject<UCAIBehaviorComponent>("Behavior");

#if WITH_EDITOR
	LabelWidget = CreateDefaultSubobject<UWidgetComponent>("Label");
	LabelWidget->SetupAttachment(GetMesh());

	ConstructorHelpers::FClassFinder<UCUserWidget_Label> LabelClass(TEXT("WidgetBlueprint'/Game/Widgets/WB_Label.WB_Label_C'"));
	TSubclassOf<UCUserWidget_Label> LabelClassType = LabelClass.Class;
	LabelWidget->SetWidgetClass(LabelClassType);
	LabelWidget->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	LabelWidget->SetDrawSize(FVector2D(120.f, 0.f));
	LabelWidget->SetWidgetSpace(EWidgetSpace::Screen);
#endif
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	LabelWidget->InitWidget();

	UCUserWidget_Label* Label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());
	Label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	Label->UpdateName(GetName());
	Label->UpdateControllerName(GetController()->GetName());
#endif
}

void ACEnemy_AI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if WITH_EDITOR
	UCUserWidget_Label* Label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());

	if (Label != nullptr)
	{
		Label->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());

		UpdateLabelRenderScale();
	}
#endif
}

void ACEnemy_AI::Hitted()
{
	Super::Hitted();
	CheckTrue(State->IsDeadMode());

	BehaviorComponent->SetHittedMode();
}

void ACEnemy_AI::End_Hitted()
{
	Super::End_Hitted();

	BehaviorComponent->SetWaitMode();
}

#if WITH_EDITOR
void ACEnemy_AI::UpdateLabelRenderScale()
{
	UCUserWidget_Label* Label = Cast<UCUserWidget_Label>(LabelWidget->GetUserWidgetObject());

	CheckNull(Label);

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector TargetLocation = GetController()->GetTargetLocation();

	float Distance = FVector::Distance(CameraLocation, TargetLocation);
	float SizeRate = 1.0f - (Distance / LabelViewAmount);

	// GEngine->AddOnScreenDebugMessage(static_cast<uint8>(DEBUG_NUMS::AI_Label), 0.1f, FColor::Red,
	// 	FString::Printf(TEXT("Distance : %f \n SizeRate : %f"), Distance, SizeRate));
	
	if (SizeRate <= 0.3f)
	{
		Label->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	Label->SetVisibility(ESlateVisibility::Visible);
	Label->SetRenderScale(FVector2D(SizeRate, SizeRate));
}
#endif