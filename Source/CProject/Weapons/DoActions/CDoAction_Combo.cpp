#include "Weapons/DoActions/CDoAction_Combo.h"

#include "Components/CStateComponent.h"
#include "Utilities/CheckMacros.h"

void UCDoAction_Combo::DoAction()
{
	CheckFalse(DoActionDatas.IsValidIndex(Index))

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(StateComponent->IsIdleMode())
	//만약 제일 위쪽에 Super::DoAction();이 있었다면, 부모의 StateComponent->SetActionMode();가 호출되어 위쪽 검사를 통과하지 못하고 여기서 멈추게 된다.  
	Super::DoAction();

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist)

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;
}

#if WITH_EDITOR
TArray<FString> UCDoAction_Combo::GetDebugInfo() const
{
	FString IndexString = "Index : " + FString::FromInt(Index);
	FString bEnableString = "Combo Enable : " + FString(bEnable ? "true" : "false");
	FString bExistString = "Combo Exist : " + FString(bExist ? "true" : "false");
	return { IndexString, bEnableString, bExistString };
}
#endif
