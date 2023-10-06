#include "Actions/CDoAction_Warp.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (AActor* actor : OwnerCharacter->Children)
	{
		if (actor->IsA<ACAttachment>() && actor->GetActorLabel().Contains("Warp"))
		{
			Decal = CHelpers::GetComponent<UDecalComponent>(actor);
			// Attachment 상속받은 Warp의 이름을 가진 액터를 가져온다 
			//IsA - Attachment를 상속받고 , 라벨 이름이 Warp 인 
			break;
		}
	}
}

void ACDoAction_Warp::DoAction()
{
	CheckFalse(*bEquipped);

	FRotator rotator;
	CheckFalse(GetCrursorLocationAndRotation(Location, rotator));

	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	Decal->SetWorldLocation(Location);
	Decal->SetWorldRotation(rotator);

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRatio, Datas[0].StartSection);

	Datas[0].bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	FTransform transform = Datas[0].EffectTransform;
	//transform.AddToTranslation(OwnerCharacter->GetActorLocation());
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Datas[0].Effect, transform);

	UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, OwnerCharacter->GetMesh(), "", transform.GetLocation(), FRotator(transform.GetRotation()), transform.GetScale3D());
}

void ACDoAction_Warp::End_DoAction()
{
	OwnerCharacter->SetActorLocation(Location);
	Location = FVector::ZeroVector;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckFalse(*bEquipped);

	FVector location;
	FRotator rotator;
	if (GetCrursorLocationAndRotation(location, rotator))
	{
		Decal->SetVisibility(true);
		Decal->SetWorldLocation(location);
		Decal->SetWorldRotation(rotator);

		return;
	}

	Decal->SetVisibility(false);
}

bool ACDoAction_Warp::GetCrursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotator)
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Enum에서 AsByte 우리가 어떤 자료형인지 명시해줘야한다.
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery1);
	// 1번이 WorldStatic을 쓰겠다 , 콜리전 쿼리에 순서가 이 숫자 순서 
	FHitResult hitResult;
	if (controller->GetHitResultUnderCursorForObjects(objects, false, hitResult))
	{
		OutLocation = hitResult.Location;
		OutRotator = hitResult.ImpactNormal.Rotation();
		// ImpactNormal은 충돌할때 잘리는걸 더 자연스럽게할수있다 


		return true;
	}
	//충돌안되면 그냥 false
	return false;
}
