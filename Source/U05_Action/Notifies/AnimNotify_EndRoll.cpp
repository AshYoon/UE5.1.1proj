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
	CheckNull(MeshComp); //null �̸� �ȵ� , null �̸� �Ÿ� 
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->End_Roll();
}



