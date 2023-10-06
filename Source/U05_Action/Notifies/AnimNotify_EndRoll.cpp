#include "AnimNotify_EndRoll.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UAnimNotify_EndRoll::GetNotifyName_Implementation() const
{
	return "Roll";
}

void UAnimNotify_EndRoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp); //null 이면 안됨 , null 이면 거름 
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Roll();
}



