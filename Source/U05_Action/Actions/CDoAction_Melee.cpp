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

	// ������ asset ������ 0���� ���ƾ��ϰ� 
	// �̰� ��� �ȉ���Ŵ� , bEnable �� Ʈ���϶� Exist�� true, enable false�� �ٲ�����ϴµ�
	// ��� �ȵǴϱ� ���� �ȵǾ����� 





	// Idle�϶����� 
	CheckFalse(State->IsIdleMode());

	State->SetActionMode();
	// DoActionData�� �ִ� ��Ÿ�� �������ְ� 
	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	// �����̸鼭 �÷��̵Ǵ� ��Ÿ������ ������ϴ°��� üũ 
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	// �츮�� �޺��� �������� �����޺��� �Ѱ����� �����޺� ������ ����Ǵ°� endaction������� 
	// �޺��� ������ ���������� ��ٷ��� begin������������ �޺��� ����¼��� begin�� �ɸ��� �޺��� �Ѿ��
	// �޺��� �Ѿ�¼��� �ٷ� �޺��� ����ǰ� ��������� ����� 

	// �θ��ִ� �ξ׼� ���� 
	Super::Begin_DoAction();

	// �޺��� �����ϴ��� Ȯ�� 
	CheckFalse(bExist);
	bExist = false;
	

	OwnerCharacter->StopAnimMontage(); // ��Ÿ�� stop 

	Index++; // �ε��� ���� 

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();


}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	// �ٽ� Idle�� 
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
	// �ϴ� ȣ��Ǵ����� Ȯ��
	CLog::Log(InOtherCharacter->GetName());


	// HittedCharacter �ȿ��ִ� �ֵ鸸ŭ For���� ������ 
	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			// ���� other�� hit�� �ֶ� �浹�ȴٸ� return�ؼ� �Լ��� �׳� ���������� 
			return;
	}
	HittedCharacters.Add(InOtherCharacter);


	//ActionData�ȿ� ��ƼŬ�� �����ͼ� 
	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform; //  TransForm�� �����ͼ� 
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);// spawn
	}

	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false) // NearlyZero -> ���ӿ����� 0�� �߾Ⱦ��ϱ� ���� 0�� ����������� ���� 
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		//SetGlobalTimeDilation -> ��ü �ð� ���ߴ°� , 1e-3f 10�� -3��, 
		// ��ü�ð���ü�� �������Ŵϱ� , �ٽ� �����ִ°� hitstop���ٰ� �츮���������� �� �ð���ŭ ��������Ѵ� 



		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}


	// ī�޶� ����ũ ���� 
	TSubclassOf<UCameraShakeBase> shake = Datas[Index].ShakeClass;
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);




	FDamageEvent e;

	// TakeDamage �����ֱ� ���� �츮������ Character�迭�ȿ� �̹� �浹�Ⱦֵ� �־ 
	// �迭�� ���� �ֵ鸸 �浹 
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
	// ���� character�� ���� �迭�� �������Ѵ� 

	HittedCharacters.Empty();


}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);


}
