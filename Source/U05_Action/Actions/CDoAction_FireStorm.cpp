#include "Actions/CDoAction_FireStorm.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Particles/ParticleSystemComponent.h"

void ACDoAction_FireStorm::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("FireStorm"))
		{
			// Label 세팅했으니 FireStorm 에러안뜸 
			Box = CHelpers::GetComponent<UBoxComponent>(actor);
			// Box 가져오기 
			break;
		}
	}
}

void ACDoAction_FireStorm::DoAction()
{
	CheckFalse(State->IsIdleMode());
	// idleMode Check후 ActionMode로 세팅 
	State->SetActionMode();
	
	// Montage 재생 
	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);
	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
	// CanMove체크 
}

void ACDoAction_FireStorm::Begin_DoAction()
{
	Angle = UKismetMathLibrary::RandomFloatInRange(0, 360);

	Attached = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, Box, "");
	Attached->SetRelativeLocation(Datas[0].EffectTransform.GetLocation());
	Attached->SetRelativeScale3D(Datas[0].EffectTransform.GetScale3D());



	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OnCollision();

	UKismetSystemLibrary::K2_SetTimer(this, "Hitted", HittedTime, true);
}

void ACDoAction_FireStorm::End_DoAction()
{
	Status->SetMove();

	FTimerDynamicDelegate timerDelegate;
	timerDelegate.BindUFunction(this, "Finish"); // Finish호출 
	UKismetSystemLibrary::K2_SetTimerDelegate(timerDelegate, Time, false);// Finish까지의Time
}

void ACDoAction_FireStorm::Finish()
{
	State->SetIdleMode();
	Attached->DestroyComponent();

	ACAttachment* attachment = Cast<ACAttachment>(Box->GetOwner());
	attachment->OffCollision();

	UKismetSystemLibrary::K2_ClearTimer(this, "Hitted");
}

void ACDoAction_FireStorm::Hitted()
{
	FDamageEvent e;
	for (ACharacter* character : HittedCharacter)
		character->TakeDamage(Datas[0].Power, e, OwnerCharacter->GetController(), this);
}

void ACDoAction_FireStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(*bEquipped);
	CheckFalse(State->IsActionMode());


	FVector location = OwnerCharacter->GetActorLocation();

	// 거의 360에 근접하면 angle 을 다시 0 으로 
	// 0~360 제한해주려고 , 360을 돌아서 회전하면 축이 겹치는 짐벌락 현상이 생길수있어서 축값을 제한 
	Angle += Speed * DeltaTime;
	if (FMath::IsNearlyEqual(Angle, 360))
		Angle = 0.0f;

	// 0~ 360에 랜덤값을 넣어주는 끊기지않고 자연스럽게 진행될꺼다 

	// 기준축에서 Distance 만큼 떨어지게 
	FVector axis = FVector(Distance, 0, 0);
	// Angle값넣어주고 value에 값들어가서 += 이걸로 값이 계속들어감 
	FVector value = axis.RotateAngleAxis(Angle, FVector(0, 0, 1));
	location += value;
	// Box에 값 넣어주기 
	Box->SetWorldLocation(location);
}

void ACDoAction_FireStorm::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacter.AddUnique(InOtherCharacter);
	// AddUnique로 Hit되는거 검출하기 
	// HIt수가 과열되는걸방지 
}

void ACDoAction_FireStorm::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	HittedCharacter.Remove(InOtherCharacter);
}
