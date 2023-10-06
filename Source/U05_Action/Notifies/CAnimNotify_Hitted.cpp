#include "Notifies/CAnimNotify_Hitted.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_Hitted::GetNotifyName_Implementation() const
{
	return "Hitted";
}

void UCAnimNotify_Hitted::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(character);
	CheckNull(state);

	state->SetIdleMode(); // 다시 Idlemode로 되돌려주기 
}
