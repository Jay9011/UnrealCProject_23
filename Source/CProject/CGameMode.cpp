#include "CGameMode.h"

#include "Global.h"
#include "Camera/CameraActor.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Character/Player/BP_CPlayer.BP_CPlayer_C'");
}

void ACGameMode::StartPlay()
{
	Super::StartPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		ACameraActor* CameraActor = nullptr;
		for (AActor* actor : GetWorld()->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->GetName() == "TestCamera")
			{
				CameraActor = Cast<ACameraActor>(actor);
				break;
			}
		}

		if (CameraActor != nullptr)
		{
			PlayerController->SetViewTargetWithBlend(CameraActor);
		}
	}
}
