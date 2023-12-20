#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugHeader.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

/**
 * @brief 무기 외형 정보와 캡슐 정보를 가지고 있는 액터
 */
UCLASS()
class CPROJECT_API ACAttachment : public AActor
{
	GENERATED_BODY()

public:	
	ACAttachment();

protected:
	virtual void BeginPlay() override;

protected:
	//AttachActorToComponent를 간소화하는 함수
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void AttachTo(FName InSocketName);
	
/*
 * 장착 관련
 */
public:
	//장착시 호출 될 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginEquip();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnUnequip();

/*
 * 충돌체 관련
 */
public:
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnCollisions();
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OffCollisions();

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	                             AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	                             bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	                           AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool CollisionChannelCheck(const UShapeComponent* InShapeComponent = nullptr);
/*
 * Delegate
 */
public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;
	
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TArray<class UShapeComponent*> Collisions;

	UPROPERTY()
	class ACGameMode* GameMode;
};
