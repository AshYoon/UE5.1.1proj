#include "Actions/CDoAction_Throw.h"
#include "Global.h"
#include "CAim.h"
#include "CThrow.h" //�����ȸ������ 
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"



void ACDoAction_Throw::BeginPlay()
{
	// state������ begin play�� �������Ѵ� 

	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);
	
	// new = C������ New object -> �𸮾� ������ �÷��Ͱ� �����ش� 
	// ���� ����⵵ �����ϴ� 

}


void ACDoAction_Throw::DoAction()
{
	// available �Ҷ� zoom �� true �϶� Actionmode�� 
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZoom())

	 // �⺻��� 
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Throw_Projectile");

	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	// �׻� ���� ���� 

	FTransform transform = Datas[0].EffectTransform; // transform ? 
	transform.AddToTranslation(location); // ȸ���� ������ذ� �־��ְ� 
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;


	params.Owner = this;

	//params.SpawnCollisionHandlingOverride;
	// �̿ɼ��� ����ϴ����� 
	// ���Ļ� �ϴٺ��� �׳� ���ڸ��� ��ġ�� �浹ü���־ ���������ϱ� 
	// �������� �ϴ°͵� �߿��ϴ� 
	// �ɼ��� ���� �����ɼ��� ���������ִ� 



	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActor<ACThrow>(Datas[0].ThrowClass, transform, params);
	// �츮���� �Ķ���Ͱ�����  ������ Spawn 
	// �߾ȸ´ٺ��� �����̾ȵɼ����ִ� 
	// �׳� ������ �����̾ƴ϶� SpawnActor ���� 


	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, params);
	// spawnactor deferred �� Ȱ���ؾ��Ѵ� , �Ķ���Ͱ� �޶��� 
	ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	// AlwaysSpawn �浹ü�� ��ġ�� ���� �ϴ� �׳� ��� 



	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);

	UGameplayStatics::FinishSpawningActor(throwObject, transform);
	// ���� ������� ���� 



}

void ACDoAction_Throw::End_DoAction()
{
	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Throw::OnAim()
{
	Aim->OnZoom();

}

void ACDoAction_Throw::OffAim()
{
	Aim->OffZoom();
}

void ACDoAction_Throw::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_Throw::OnThrowBeginOverlap(FHitResult InHitResult)
{
	// �浹 �Ǿ����� 
	// �ϴ��� �������� �ִ°ɷ� 
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}
