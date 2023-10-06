#include "Components/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"






UCFeetComponent::UCFeetComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	// owner character �ϴ� �������� 
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}



void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// �ᱹ �������� ������ AnimInstance���� ���ִ°Ŵ� 
	float leftDistance;
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation);
	// �ٷιٷ� ���� ������ �Ҷ� ����ϱ� ���⵵ interpolation ������ ������Ѵ� 

	// �������̶� �޹� ��  �߿� �Ʒ����� ����ؼ� �����ؾ��ϴϱ� offset���� ���� 
	float rightDistance;
	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation);



	// ������ ���� 
	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);


	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);


	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	// �����̳ʵ��� �����ʿ����� �Ϻ��� ��Ī������ ����ϱ⋚���� ������ �Ķ���͸� �־��־���Ѵ� 

	// ȸ������ ���� ������Ѵ� 
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);








}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	// ���⿡ ������ ������ �Ǿ��� ������ data�� ����Ǵ°Ŵϱ� 
	OutDistance = 0.0f;


	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket); // ��Ĺ ���������ؼ� ��ġ�������� 
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z); // Start ������ ������ ����Ѵ� 

	float traceZ = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance; // Capsule�� HafHeight �� TraceDistance ��ŭ - ���ش� 
	FVector end = FVector(location.X, location.Y, traceZ);

	//DrawDebugLine(GetWorld(), start, end, FColor::Green);
	// ���θ����ѰŰ� 

	// LineTrace�� �Ἥ ���� ��������Ѵ� 

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		true, ignoreActors, DrawDebugType, hitResult, true, FLinearColor::Green, FLinearColor::Red);
	// �浹�Ǹ� Red , �̰� ������ 

	//TEnumAsByte
		// defualt�� none������ TEnumAsByte��� Byte�� ����Ѵٰ� ���������Ѵ�
	//

	if (hitResult.GetActor())
	{
		float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
		OutDistance = OffsetDistance + length - TraceDistance;



		//����� ź��Ʈ Sin�� Y�� X�� �ڻ����ε� 

		// �������� Yaw , ���ư��������� �����ʴ´� 

		// roll ź��Ʈ�� ������ ��� , 

		// ���Ը��ϸ�Ǵϱ� ���� �ѹ� �ؼ��غ��� 
		FVector normal = hitResult.ImpactNormal;
		float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
		float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

		OutRotation = FRotator(pitch, 0.0f, roll);
	}



}