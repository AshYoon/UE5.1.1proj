# UE5.1.1proj
Migration from UE 4.27.2 


## CPlayer

**설명**: Component를 제외한 기능들을 구현했습니다 . KeyBinding과 LineTrace를 이용한 FootStep과 ItemInteraction을 구현했습니다 

### 관련 함수 혹은 코드

[FootStep 시연영상 ](https://www.youtube.com/watch?v=xjgq64Y3Bdw)
Unreal Engine C++ 에서 LineTrace와 GetSurfaceType 으로 
Audio cue 를 이용해서 Physical Material 마다 다른 FootStep사운드가 재생
Mabinogi Foot Step sound를 가져와서 사용했습니다  

```cpp
void ACPlayer::FootStep()
{

	GetActorLocation();
	FVector MinusVector(0.f, 0.f,150.0f);
	FVector FootTraceStart{  GetActorLocation() };
	FVector FootTraceEnd{ FootTraceStart -  MinusVector};
	FHitResult HitResult;
	FCollisionQueryParams CQP = FCollisionQueryParams::DefaultQueryParam;
	CQP.AddIgnoredActor(this);
	bool bReturnPhysicalMaterial = CQP.bReturnPhysicalMaterial;
	bReturnPhysicalMaterial = true;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(this);


	DrawDebugLine(GetWorld(), FootTraceStart, FootTraceEnd, FColor::Blue, false, 1.0f, 0, 2.0f);

	// 
	UKismetSystemLibrary::LineTraceSingle(GetWorld(),FootTraceStart,FootTraceEnd,UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, ignoreActors, EDrawDebugTrace::Type::None, HitResult, true, FColor::Green, FColor::Blue, 1.0f);



	// sound asset is available - snow , rock , soil , dungeon , water , glass, grass  ,wood
	// Dungeon , Water , Soil , Rock , Snow , Wood ,Grass , Glass 

	// nullcheck ? 
	if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType1)
	{
		CLog::Print("Dungeon", -1, 10.0f, FColor::Blue);

		FootAudioComponent->SetIntParameter("foot", 0);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType2)
	{
		CLog::Print("Water", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 1);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType3)
	{
		CLog::Print("Soil", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 2);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType4)
	{
		CLog::Print("Rock", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 3);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType5)
	{
		CLog::Print("Snow", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 4);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType6)
	{
		CLog::Print("Wood", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 5);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType7)
	{
		CLog::Print("Grass", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 6);
		FootAudioComponent->Play(0.f);
	}
	else if (UGameplayStatics::GetSurfaceType(HitResult) == EPhysicalSurface::SurfaceType7)
	{
		CLog::Print("Glass", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 7);
		FootAudioComponent->Play(0.f);
	}
````

[ItemInteraction시연영상](https://www.youtube.com/watch?v=HXcDLyzUIIc)
 


````
void ACPlayer::PerformInteractionCheck()
{
	// counting time begin 
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	//little bit more performence than = , this will use with aiming mode 
	//when you initialize with brackets ({ }) extremely type safe becasuse it guarantees that type you're
	// using ,is going to get initialized ,bracket initialize = very type safe
	//FVector TraceStart{ FVector::ZeroVector }; 
	FVector TraceStart {GetPawnViewLocation()  }; // initialize pawn eye  



	// Get ViewRotation convert to vector and multiploe with our checkdistance and add to Trace start 
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };


	float LookDirection =  FVector::DotProduct(GetActorForwardVector(),GetViewRotation().Vector()); 

	/* if pointing positive , pointing same direction , then do linetrace  , compare Character vector and view vector */
	// view angle of interaction check is 180 degree
	if (LookDirection > 0)
	{
		// contains usefulThings for lineTraces
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);//ignore myself 
		FHitResult TraceHit; //stored actor is hitted 
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{


			/* after we hit  check  this is inheritance with our interface */
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{




				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					// 
					FoundInteractable(TraceHit.GetActor());
					return;
				}




				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{


					return;
				}

			}
		}
	}

	NoInteractableFound();

}
````

## CPlayer

**설명**: DataTable의 Action Data 기반의 Battle Mode 

### 관련 함수 혹은 코드

```cpp
ACPlayer::ACPlayer()
	ActionList->GetData(0).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFist);
	ActionList->GetData(1).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnOneHand);
	ActionList->GetData(2).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnTwoHand);
	ActionList->GetData(3).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnWarp);
	ActionList->GetData(4).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFireStorm);
	ActionList->GetData(5).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnIceBall);


void UCActionData::BeginPlay(ACharacter* InOwnerCharacter, UCAction** OutAction)
{
	FTransform transform;


	ACAttachment* attachment = NULL; // null 값으로 초기화 



	// 어떻게 보면 직업자체를 데이터 에셋으로 관리한다고 보면된다  
	//Attachment
	if(!!AttachmentClass)
	{
		attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>(AttachmentClass, transform, InOwnerCharacter);
		attachment->SetActorLabel(GetLableName(InOwnerCharacter, "Attachment"));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}


	ACEquipment* equipment = NULL;



	//Equipment
	if(!!EquipmentClass)
	{
		equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, InOwnerCharacter);
		equipment->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		equipment->SetActorLabel(GetLableName(InOwnerCharacter, "Equipment"));
		equipment->SetData(EquipmentData);
		equipment->SetColor(EquipmentColor);
		UGameplayStatics::FinishSpawningActor(equipment, transform);
		if (!!AttachmentClass)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnequipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}


	ACDoAction* doAction = NULL;

	//DoAction
	if(!!DoActionClass)
	{
	   doAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>(DoActionClass, transform, InOwnerCharacter);
	   doAction->AttachToComponent(InOwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	   doAction->SetActorLabel(GetLableName(InOwnerCharacter, "DoAction"));
	   doAction->SetDatas(DoActionDatas);
	   UGameplayStatics::FinishSpawningActor(doAction, transform);

	   if (!!equipment)
	   {
		   doAction->SetEquipped(equipment->GetEquipped());
	   }



	   if (!!attachment)
	   {
		   // Attachment의 overlap과 collision을 DoActionClass에 연결 
		   attachment->OnAttamentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
		   attachment->OnAttamentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);

		   attachment->OnAttachmentCollision.AddDynamic(doAction, &ACDoAction::OnAttachmentCollision);
		   attachment->OffAttachmentCollision.AddDynamic(doAction, &ACDoAction::OffAttachmentCollision);
	   }


	}

	// 

	// 리턴할때 new object로 하는거지 action으로 
	//가져다가 쓸때마다 복사된다 , 실제 스탯은 우리가 안에서 만들어서 정하고 , 이거 기반으로 DA 만들어서 정하니깐
	// 애를 호출하는건 Action Component 

	*OutAction = NewObject<UCAction>();
	(*OutAction)->Attachment = attachment;
	(*OutAction)->Equipment = equipment;
	(*OutAction)->DoAction = doAction;
	(*OutAction)->EquipmentColor = EquipmentColor;

}
````
[Action시연영상](https://www.youtube.com/watch?v=4v_8msfKJ_A)
Unreal Engine C++ 에서 Action DataBase기반의 Action을 가져와서 
해당 직업클래스의 Action을 취할수있게해줍니다 


## FeetComponent

**설명**: LineTrace와 기울기를 구해서 IK 구현 
[Action시연영상](https://www.youtube.com/watch?v=LrjqitSjbw)
### 관련 함수 혹은 코드
```cpp
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
````


