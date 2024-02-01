#include "Notify/CAnimNotify_MakeNoise.h"

#include "Character/CBaseCharacter.h"
#include "Components/CActorSoundComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CheckMacros.h"

FString UCAnimNotify_MakeNoise::GetNotifyName_Implementation() const
{
	return "FootStep Noise";
}

void UCAnimNotify_MakeNoise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);

	ACBaseCharacter* Owner = Cast<ACBaseCharacter>(MeshComp->GetOwner());
	CheckNull(Owner);

	UCActorSoundComponent* SoundComponent = Cast<UCActorSoundComponent>(Owner->GetComponentByClass(UCActorSoundComponent::StaticClass()));
	CheckNull(SoundComponent);

	FVector Start, End;
	Start = Owner->GetActorLocation();
	End = Start - FVector::UpVector * 100.f;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Owner);
	
	FHitResult HitResult;
	if(UKismetSystemLibrary::LineTraceSingle(Owner, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::None, HitResult, true))
	{
		EPhysicalSurface Surface = UGameplayStatics::GetSurfaceType(HitResult);
		FFootStepRowData* FootStepRowData = SoundComponent->GetFootStepRowData(Surface);
		CheckNull(FootStepRowData);
		
		UFootStepSoundManager::PlayFootStepSound(Owner, HitResult.Location, *FootStepRowData, SoundData);
	}
	
}
