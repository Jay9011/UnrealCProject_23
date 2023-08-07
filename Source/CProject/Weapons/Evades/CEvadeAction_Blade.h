#pragma once

#include "CoreMinimal.h"
#include "Weapons/CEvadeAction.h"
#include "CEvadeAction_Blade.generated.h"

/**
 * @brief Blade 회피 클래스
 */
UCLASS()
class CPROJECT_API UCEvadeAction_Blade : public UCEvadeAction
{
private:
	GENERATED_BODY()
	
public:
	virtual void OnBeginEquip() override;
	virtual void OnUnequip() override;

public:
	virtual void BeginEvade() override;
	virtual void EndEvade() override;

};
