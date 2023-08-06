#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

/**
 * @brief 무기 외형 관련 액터
 */
UCLASS()
class CPROJECT_API ACAttachment : public AActor
{
	GENERATED_BODY()

public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	//장착시 호출 될 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginEquip();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();
	
protected:
	//AttachActorToComponent를 간소화하는 함수
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void AttachTo(FName InSocketName);
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	class ACharacter* OwnerCharacter;
};
