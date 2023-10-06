#include "Notifies/CAnimNotifyState_Dead.h"

#include "Global.h"
#include "Characters/ICharacter.h"

FString UCAnimNotifyState_Dead::GetNotifyName_Implementation() const
{
	//노티파이 이름
	return "Dead";
}

void UCAnimNotifyState_Dead::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	// Null 확인 
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->Begin_Dead(); // begindead 호출 

}

void UCAnimNotifyState_Dead::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacter* character = Cast<IICharacter>(MeshComp->GetOwner());
	CheckNull(character);

	character->End_Dead();
}