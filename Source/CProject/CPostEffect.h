#pragma once

#include "CoreMinimal.h"

/**
 * @brief Post Effect 데이터를 관리하는 클래스
 */
class CPROJECT_API CPostEffect
{
public:
	CPostEffect();
	~CPostEffect();

	void TickTimeline(float DeltaTime);
};
