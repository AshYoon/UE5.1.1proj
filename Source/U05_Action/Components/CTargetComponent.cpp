#include "CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	//  ParticleSystem'/Game/Effects/P_Enrage_Base.P_Enrage_Base'
	CHelpers::GetAsset<UParticleSystem>(&Particle, "ParticleSystem'/Game/Effects/P_Enrage_Base.P_Enrage_Base'");


}



void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	// OwnerCharacter 세팅 
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::ToggleTarget()
{
	if (!!Target) // 주변에 타겟이없으면 EndTarget
	{
		EndTargeting();

		return;
	}

	StartTargeting();


}

void UCTargetComponent::ChangeTargetLeft()
{
	ChangeTarget(false);
	// 원래는 탭만쓰는데 , 그냥 우리가 키보드키로 정할수있게 만들었다 

}

void UCTargetComponent::ChangeTargetRight()
{
	ChangeTarget(true);


}

void UCTargetComponent::StartTargeting()
{

	SetTraceTargets();
	SetTarget();

}

void UCTargetComponent::EndTargeting()
{

	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();


}

void UCTargetComponent::SetTraceTargets()
{
	// 우리가 키를 누르면 대상이 검출이 되야한다 




	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius, "Pawn", false, ignoreActors, Debug, hitResults, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	for (const FHitResult& result : hitResults)
	{
		if (result.GetActor()->GetClass() == OwnerCharacter->GetClass())
			continue;


		ACharacter* character = Cast<ACharacter>(result.GetActor());

		if (!!character)
			TraceTargets.AddUnique(character);
	}


	for (const ACharacter* character : TraceTargets)
		CLog::Print(character->GetName());



}

void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : TraceTargets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		offset = offset.GetSafeNormal();

		float temp = direction | offset;
		if (temp < angle)
			continue;

		// angle에 계속 값이 들어감 모든angle에 내적값이 계속들어감 
		angle = temp;
		target = character;
	}


	//CLog::Print(target->GetActorLabel());


	ChangeCursor(target);








}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	// 나를기준으로 가까운거리 , 혹은 Target을 기준으로 가까운거리 로 채인지한다 
	// TMap으로 저장을 한다음에 그걸 Character 를 돌려서 Target위치 가져오고 좌우기준으로 구하기 
	// 두 벡터에 수직인거 


	CheckNull(Target); // 타겟이 있어야하고 

	TMap<float, ACharacter*> map; // map 만들기 
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue; // 지금설정된 캐릭터는 제외 

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTarget = targetLocation - ownerLocation; // 우리로케이션에서 Target을 향하는 벡터 가 나온다 

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation()); // 쿼터니언 
		FVector forward = quat.GetForwardVector(); // 전방벡터 구하기 
		FVector up = quat.GetUpVector(); // 업벡터 

		FVector cross = forward ^ ownerToTarget; // 내적 
		float dot = cross | up; 

		map.Add(dot, character); // 이걸 캐릭터마다 전부 저장 
	}


	float minimum = FLT_MAX; // 미니멈 수 
	ACharacter* target = NULL; 

	TArray<float> keys; // 거리 
	map.GetKeys(keys); // key는 거리 value는 캐릭터 
	for (float key : keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) > minimum) // 음수양수 상관없이 가까운에 
			continue;

		minimum = FMath::Abs(key); // 절대값 
		target = *map.Find(key); // 해당 키 접근해서 character 구하기 
	}

	ChangeCursor(target); // 그 타겟으로 체인지 



}

void UCTargetComponent::ChangeCursor(ACharacter * InTarget)
{
	//EndTarget은 그냥 비우는ㄴ거다 
	if (!!InTarget) // 타겟이 있다면 
	{
		if (!!Attached) // 붙어있는거 있으면 기존에있는거 지우는거 
			Attached->DestroyComponent(); // 기존에있던거 지우기 

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InTarget->GetMesh(), "Spine_Target");
		Target = InTarget;

		return;
	}

	EndTargeting();



}



void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 커서가 끝까지가지않는다 카메라가 안돌아서 그런거같다 
	CheckNull(Target);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	bool b = false;
	b |= state->IsDeadMode();
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;

	if (b == true)
	{
		EndTargeting();

		return;
	}

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InteropSpeed);
	// 이게 인터폴레이션 하는거 , 현재 시간이랑 이런거 해서 보간해주는거 - !!!! 꼭 알아보자 

	OwnerCharacter->GetController()->SetControlRotation(rotator);
	//여기서 Controller는 카메라 
	// 너무 뚝뚝 끊긴다 
	// 보간 관련된거도 다양한 보간방법에대해서 알필요가있다 . 언리얼에 대한 기술 최적화 보간 이런거 
	// TraceTarget
	// 기술적으로 접근해야한다 , 로우폴리곤이라던가 
	// 네비메쉬 동작원리는뭔지 , Targeting만해도 내적외적 벡터 3가지 기본적인걸 알아야한다












}

