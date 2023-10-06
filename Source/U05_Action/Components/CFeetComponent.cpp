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

	// owner character 일단 가져오고 
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}



void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 결국 최종적인 조정은 AnimInstance에서 해주는거다 
	float leftDistance;
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation);
	// 바로바로 발을 내리면 뚝뚝 끊기니깐 여기도 interpolation 보간을 해줘야한다 

	// 오른발이랑 왼발 그  중에 아랫값을 사용해서 조정해야하니깐 offset으로 조정 
	float rightDistance;
	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation);



	// 오프셋 조정 
	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);


	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);


	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);
	// 디자이너들은 오른쪽왼쪽이 완벽한 대칭구조로 사용하기떄문에 음수로 파라미터를 넣어주어야한다 

	// 회전값도 보간 해줘야한다 
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);








}

void UCFeetComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	// 여기에 값들이 리턴이 되야지 실제로 data가 적용되는거니깐 
	OutDistance = 0.0f;


	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket); // 소캣 기준으로해서 위치가져오고 
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z); // Start 지점을 지축을 사용한다 

	float traceZ = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance; // Capsule의 HafHeight 에 TraceDistance 만큼 - 뺴준다 
	FVector end = FVector(location.X, location.Y, traceZ);

	//DrawDebugLine(GetWorld(), start, end, FColor::Green);
	// 라인만구한거고 

	// LineTrace를 써서 직접 내려줘야한다 

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), 
		true, ignoreActors, DrawDebugType, hitResult, true, FLinearColor::Green, FLinearColor::Red);
	// 충돌되면 Red , 이거 가보면 

	//TEnumAsByte
		// defualt는 none이지만 TEnumAsByte라고 Byte로 사용한다고 명시해줘야한다
	//

	if (hitResult.GetActor())
	{
		float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
		OutDistance = OffsetDistance + length - TraceDistance;



		//기울기는 탄젠트 Sin이 Y로 X가 코사인인데 

		// 수직축이 Yaw , 돌아갈순있지만 쓰지않는다 

		// roll 탄젠트는 역기울기 사용 , 

		// 대입만하면되니깐 내가 한번 해석해보기 
		FVector normal = hitResult.ImpactNormal;
		float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
		float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

		OutRotation = FRotator(pitch, 0.0f, roll);
	}



}