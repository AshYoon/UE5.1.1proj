#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"





void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	//ActionType Change를 AddDynamic으로 연결시켜주기 
	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(character);
	CheckNull(action);

	action->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);

	
	



}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	// 속도 와 방향 구하기 
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* character = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(character);

	Speed = character->GetVelocity().Size2D();
	Direction = CalculateDirection(character->GetVelocity(), character->GetControlRotation());



	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(character);
	if (!!feet)
		FeetData = feet->GetData();

}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}
