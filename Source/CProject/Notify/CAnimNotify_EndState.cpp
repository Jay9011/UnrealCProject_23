#include "CAnimNotify_EndState.h"

#include "Components/IStateNotify.h"
#include "Utilities/CheckMacros.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp)
	CheckNull(MeshComp->GetOwner())

	IIStateNotify* StateNotify = Cast<IIStateNotify>(MeshComp->GetOwner());
	CheckNull(StateNotify)
	
	switch (StateType)
	{
	case EStateType::Evade:
		StateNotify->End_Evade();
		break;
	}
}
