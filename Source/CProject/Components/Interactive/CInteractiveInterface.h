#pragma once

#include "CoreMinimal.h"
#include "Character/CPlayer.h"
#include "UObject/Interface.h"
#include "CInteractiveInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UCInteractiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	@brief CInteractiveComponent의 인터페이스 
 */
class CPROJECT_API ICInteractiveInterface
{
	GENERATED_BODY()

public:
	/**
	 *	@brief 상호작용을 시작한다.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	void OnInteract(ACPlayer* InInteractPlayer);
	virtual void OnInteract_Implementation(ACPlayer* InInteractPlayer) = 0;
	

	/**
	 *	@brief 상호작용 가능 여부를 갱신한다.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	bool CanInteract(ACPlayer* InInteractPlayer);
	virtual bool CanInteract_Implementation(ACPlayer* InInteractPlayer) = 0;

	/**
	 *	@brief 상호작용 대상을 반환한다.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	AActor* GetInteractTarget();
	virtual AActor* GetInteractTarget_Implementation() = 0;

	/**
	 *	@brief 상호작용 텍스트를 반환한다.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactive")
	FText GetInteractText();
	virtual FText GetInteractText_Implementation() = 0;
};
