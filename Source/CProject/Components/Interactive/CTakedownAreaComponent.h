#pragma once

#include "CoreMinimal.h"
#include "CInteractiveInterface.h"
#include "Components/SceneComponent.h"
#include "CTakedownAreaComponent.generated.h"


class ACPlayer;
class UCAIBehaviorComponent;
class ACEnemy_AI;
class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCTakedownAreaComponent : public USceneComponent
	, public ICInteractiveInterface
{
	GENERATED_BODY()

public:	
	UCTakedownAreaComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void OnTakedownAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTakedownAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTakedownMontage(UAnimMontage* InMontage);
	
private:
	bool CheckStealthKillable(ACPlayer* InPlayer);
	bool IsEnemyInStealthKillableState();

	UFUNCTION()
	void OnTakedownEnd();
	
private:
	UPROPERTY(EditAnywhere, Category = "TakedownArea")
	UBoxComponent* TakedownArea = nullptr;

	UPROPERTY()
	ACEnemy_AI* OwnerEnemy = nullptr;

	UPROPERTY()
	UCAIBehaviorComponent* OwnerBehaviorComponent = nullptr;

	/*
	 * ICInteractiveInterface
	 */
public:
	virtual void OnInteract(ACPlayer* InInteractPlayer) override;
	virtual bool CanInteract(ACPlayer* InInteractPlayer) override;
	virtual AActor* GetInteractTarget() override;
	virtual FText GetInteractText() override;
};
