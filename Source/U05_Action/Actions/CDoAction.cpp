#include "Actions/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACDoAction::ACDoAction()
{
 	PrimaryActorTick.bCanEverTick = true;

}


void ACDoAction::BeginPlay()
{
	// 생성자가아닌 BeginPlay 전 시점에 다 만들어져있어야한다 
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);


	Super::BeginPlay();
	
}


void ACDoAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

