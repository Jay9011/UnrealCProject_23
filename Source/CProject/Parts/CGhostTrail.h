#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

UCLASS()
class CPROJECT_API ACGhostTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	ACGhostTrail();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
public:
	void InitData(const FColor& InGhostTrailColor = FColor::White, const ACharacter* InOwner = nullptr);
	bool Start(const FTransform& InTransform);
	void End();
	void SetOpacity(UCurveFloat* InOpacityCurve, const float InDelayTime);

private:
	void Show();
	void Hide();
	UFUNCTION()
	void OpacityTimelineProgress(float InValue);
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UPoseableMeshComponent* PoseableMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInstance* GhostTrailMaterialClass;

	UPROPERTY()
	UMaterialInstanceDynamic* GhostTrailMaterialInstance = nullptr;

	UPROPERTY()
	const ACharacter* OwnerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	FName ColorName = "Color";
	FColor GhostTrailColor = FColor::White;

	UPROPERTY(EditDefaultsOnly, Category = "GhostTrail")
	FName OpacityName = "Opacity";

private:
	FTimeline OpacityTimeline;
};
