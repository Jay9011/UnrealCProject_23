#pragma once

#include "CoreMinimal.h"
#include "CInteractiveInterface.h"
#include "Components/ActorComponent.h"
#include "CInteractiveComponent.generated.h"

class ACPlayer;
class UCUserWidget_interact;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPROJECT_API UCInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractiveComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	 * @brief 상호 작용 정보를 갱신합니다. (단, 해당 함수로 상호작용 해제를 할 수 없습니다.)
	 * @see 상호 작용 해제는 ClearTarget 함수를 사용합니다.
	 * @param InTarget Widget을	표시할 대상
	 * @param InInteractiveTarget 상호작용 인터페이스가 존재하는 객체
	 */
	void SetTarget(AActor* InTarget, ICInteractiveInterface* InInteractiveTarget);
	
	void ClearTarget(ICInteractiveInterface* InInteractiveTarget);
	void CheckTarget();

	UFUNCTION()
	void OnInteract();
	void OnChangeTarget(AActor* InTarget);

	/*
	 * Widget
	 */
private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UCUserWidget_interact> WidgetClass;

	UPROPERTY()
	UCUserWidget_interact* Widget = nullptr;

	/*
	 * Member
	 */
private:
	UPROPERTY()
	ACPlayer* OwnerPlayer = nullptr;
	UPROPERTY()
	APlayerController* OwnerPC = nullptr;
	
	TWeakObjectPtr<AActor> WidgetTarget;
	TWeakObjectPtr<UObject> InteractiveTarget;

	/*
	 * Getter
	 */
public:
	FORCEINLINE ACPlayer* GetOwnerPlayer() const { return OwnerPlayer; }
	FORCEINLINE AActor* GetTarget() const { return WidgetTarget.Get(); }
	FORCEINLINE ICInteractiveInterface* GetInteractiveTarget() const;
};
