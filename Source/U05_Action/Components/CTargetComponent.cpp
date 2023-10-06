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
	// OwnerCharacter ���� 
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::ToggleTarget()
{
	if (!!Target) // �ֺ��� Ÿ���̾����� EndTarget
	{
		EndTargeting();

		return;
	}

	StartTargeting();


}

void UCTargetComponent::ChangeTargetLeft()
{
	ChangeTarget(false);
	// ������ �Ǹ����µ� , �׳� �츮�� Ű����Ű�� ���Ҽ��ְ� ������� 

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
	// �츮�� Ű�� ������ ����� ������ �Ǿ��Ѵ� 




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

		// angle�� ��� ���� �� ���angle�� �������� ��ӵ� 
		angle = temp;
		target = character;
	}


	//CLog::Print(target->GetActorLabel());


	ChangeCursor(target);








}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	// ������������ �����Ÿ� , Ȥ�� Target�� �������� �����Ÿ� �� ä�����Ѵ� 
	// TMap���� ������ �Ѵ����� �װ� Character �� ������ Target��ġ �������� �¿�������� ���ϱ� 
	// �� ���Ϳ� �����ΰ� 


	CheckNull(Target); // Ÿ���� �־���ϰ� 

	TMap<float, ACharacter*> map; // map ����� 
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue; // ���ݼ����� ĳ���ʹ� ���� 

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTarget = targetLocation - ownerLocation; // �츮�����̼ǿ��� Target�� ���ϴ� ���� �� ���´� 

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation()); // ���ʹϾ� 
		FVector forward = quat.GetForwardVector(); // ���溤�� ���ϱ� 
		FVector up = quat.GetUpVector(); // ������ 

		FVector cross = forward ^ ownerToTarget; // ���� 
		float dot = cross | up; 

		map.Add(dot, character); // �̰� ĳ���͸��� ���� ���� 
	}


	float minimum = FLT_MAX; // �̴ϸ� �� 
	ACharacter* target = NULL; 

	TArray<float> keys; // �Ÿ� 
	map.GetKeys(keys); // key�� �Ÿ� value�� ĳ���� 
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

		if (FMath::Abs(key) > minimum) // ������� ������� ���� 
			continue;

		minimum = FMath::Abs(key); // ���밪 
		target = *map.Find(key); // �ش� Ű �����ؼ� character ���ϱ� 
	}

	ChangeCursor(target); // �� Ÿ������ ü���� 



}

void UCTargetComponent::ChangeCursor(ACharacter * InTarget)
{
	//EndTarget�� �׳� ���¤��Ŵ� 
	if (!!InTarget) // Ÿ���� �ִٸ� 
	{
		if (!!Attached) // �پ��ִ°� ������ �������ִ°� ����°� 
			Attached->DestroyComponent(); // �������ִ��� ����� 

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InTarget->GetMesh(), "Spine_Target");
		Target = InTarget;

		return;
	}

	EndTargeting();



}



void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Ŀ���� �����������ʴ´� ī�޶� �ȵ��Ƽ� �׷��Ű��� 
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
	// �̰� ���������̼� �ϴ°� , ���� �ð��̶� �̷��� �ؼ� �������ִ°� - !!!! �� �˾ƺ��� 

	OwnerCharacter->GetController()->SetControlRotation(rotator);
	//���⼭ Controller�� ī�޶� 
	// �ʹ� �Ҷ� ����� 
	// ���� ���õȰŵ� �پ��� ������������ؼ� ���ʿ䰡�ִ� . �𸮾� ���� ��� ����ȭ ���� �̷��� 
	// TraceTarget
	// ��������� �����ؾ��Ѵ� , �ο��������̶���� 
	// �׺�޽� ���ۿ����¹��� , Targeting���ص� �������� ���� 3���� �⺻���ΰ� �˾ƾ��Ѵ�












}

