#pragma once

#include "CoreMinimal.h"
#include "Character/CPlayer.h"
#include "UObject/Interface.h"
#include "CInteractiveInterface.generated.h"

UINTERFACE(MinimalAPI)
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
	virtual void OnInteract(ACPlayer* InInteractPlayer) = 0;

	/**
	 *	@brief 상호작용 가능 여부를 갱신한다.
	 */
	virtual bool CanInteract(ACPlayer* InInteractPlayer) = 0;

	/**
	 *	@brief 상호작용 대상을 반환한다.
	 */
	virtual AActor* GetInteractTarget() = 0;

	/**
	 *	@brief 상호작용 텍스트를 반환한다.
	 */
	virtual FText GetInteractText() = 0;
};
