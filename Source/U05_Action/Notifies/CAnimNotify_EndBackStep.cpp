#include "CAnimNotify_EndBackStep.h"
#include "Characters/CPlayer.h"
#include "Global.h"

FString UCAnimNotify_EndBackStep::GetNotifyName_Implementation() const
{
	return "Backstep";
}

void UCAnimNotify_EndBackStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->ChangeColor(FColor::Green);
	CLog::Log("EndStep");

	player->End_Backstep();
}
