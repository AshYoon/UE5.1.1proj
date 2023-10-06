#include "Actions/CDoAction_Throw.h"
#include "Global.h"
#include "CAim.h"
#include "CThrow.h" //아직안만들었음 
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"



void ACDoAction_Throw::BeginPlay()
{
	// state세팅을 begin play에 만들어야한다 

	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);
	
	// new = C에서의 New object -> 언리얼 가비지 컬렉터가 지워준다 
	// 직접 지우기도 가능하다 

}


void ACDoAction_Throw::DoAction()
{
	// available 할때 zoom 이 true 일때 Actionmode로 
	if (Aim->IsAvaliable())
		CheckFalse(Aim->IsZoom())

	 // 기본모드 
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Throw::Begin_DoAction()
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Throw_Projectile");

	FRotator rotator = OwnerCharacter->GetController()->GetControlRotation();
	// 항상 전방 방향 

	FTransform transform = Datas[0].EffectTransform; // transform ? 
	transform.AddToTranslation(location); // 회전값 만들어준거 넣어주고 
	transform.SetRotation(FQuat(rotator));

	FActorSpawnParameters params;


	params.Owner = this;

	//params.SpawnCollisionHandlingOverride;
	// 이옵션을 써야하는이유 
	// 겹쳐사 하다보면 그냥 쏘자마자 위치에 충돌체가있어서 터져버리니깐 
	// 버그재현 하는것도 중요하다 
	// 옵션을 보면 스폰옵션이 여러가지있다 



	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActor<ACThrow>(Datas[0].ThrowClass, transform, params);
	// 우리넣은 파라미터값으로  무조건 Spawn 
	// 잘안맞다보면 생성이안될수도있다 
	// 그냥 무조건 생성이아니라 SpawnActor 말고 


	//ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, params);
	// spawnactor deferred 를 활용해야한다 , 파라미터가 달라짐 
	ACThrow* throwObject = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACThrow>(Datas[0].ThrowClass, transform, OwnerCharacter, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	// AlwaysSpawn 충돌체가 겹치던 뭐던 일단 그냥 쏘기 



	throwObject->OnThrowBeginOverlap.AddDynamic(this, &ACDoAction_Throw::OnThrowBeginOverlap);

	UGameplayStatics::FinishSpawningActor(throwObject, transform);
	// 무슨 기능인지 보기 



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
	// 충돌 되었을때 
	// 일단은 데미지만 주는걸로 
	FDamageEvent e;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}
