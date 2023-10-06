#include "Actions/CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"



void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse(Datas.Num() > 0);
	if (bEnable == true)
	{
		bExist = true;
		bEnable = false;

		return;
	}

	// 데이터 asset 갯수가 0보다 많아야하고 
	// 이게 없어서 안됬던거다 , bEnable 이 트루일때 Exist가 true, enable false로 바꿔줘야하는데
	// 계속 안되니깐 지금 안되었던것 





	// Idle일때실행 
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	// DoActionData에 있는 몽타주 세팅해주고 
	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	// 움직이면서 플레이되는 몽타주인지 멈춰야하는건지 체크 
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	// 우리가 콤보를 만들어놓고 다음콤보로 넘겼을때 다음콤보 동작이 실행되는걸 endaction에만들면 
	// 콤보가 끝나고 다음꺼까지 기다렸음 begin을만들어놓으면 콤보를 벗기는순간 begin에 걸리면 콤보가 넘어가게
	// 콤보가 넘어가는순간 바로 콤보가 실행되게 만들기위해 만든것 

	// 부모에있는 두액션 실행 
	Super::Begin_DoAction();

	// 콤보가 존재하는지 확인 
	CheckFalse(bExist);
	bExist = false;
	

	OwnerCharacter->StopAnimMontage(); // 몽타주 stop 

	Index++; // 인덱스 증가 

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();


}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	// 다시 Idle로 
	State->SetIdleMode();
	Status->SetMove();

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	Index = 0;

}
void ACDoAction_Melee::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{

	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);
	// 일단 호출되는지만 확인
	CLog::Log(InOtherCharacter->GetName());


	// HittedCharacter 안에있는 애들만큼 For문을 돌려서 
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			// 만일 other랑 hit한 애랑 충돌된다면 return해서 함수를 그냥 끝내버린다 
			return;
	}
	HittedCharacters.Add(InOtherCharacter);


	//ActionData안에 파티클을 가져와서 
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform; //  TransForm도 가져와서 
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);// spawn
	}

	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // NearlyZero -> 게임에서는 0을 잘안쓰니깐 거의 0에 가까워졌을때 실행 
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		//SetGlobalTimeDilation -> 전체 시간 멈추는거 , 1e-3f 10의 -3승, 
		// 전체시간자체가 느려진거니깐 , 다시 돌려주는건 hitstop에다가 우리가느려지게 한 시간만큼 곱해줘야한다 



		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}


	// 카메라 쉐이크 실행 
	TSubclassOf<UCameraShakeBase> shake = Datas[Index].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);




	FDamageEvent e;

	// TakeDamage 보내주기 전에 우리가만든 Character배열안에 이미 충돌된애들 넣어서 
	// 배열에 없는 애들만 충돌 
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this);

}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter * InAttacker, AActor * InAttackCauser, ACharacter * InOtherCharacter)
{



}

void ACDoAction_Melee::OnAttachmentCollision()
{



}

void ACDoAction_Melee::OffAttachmentCollision()
{
	// 때린 character를 넣은 배열을 비워줘야한다 

	HittedCharacters.Empty();


}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);


}
