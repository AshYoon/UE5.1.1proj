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
switch (UGameplayStatics::GetSurfaceType(HitResult))
	{
	case::EPhysicalSurface::SurfaceType1:
		CLog::Print("Dungeon", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 0);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType2:
		CLog::Print("Water", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 1);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType3:
		CLog::Print("Soil", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 2);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType4:
		CLog::Print("Rock", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 3);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType5:
		CLog::Print("Snow", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 4);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType6:
		CLog::Print("Wood", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 5);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType7:
		CLog::Print("Grass", -1, 10.0f, FColor::Blue);
			FootAudioComponent->SetIntParameter("foot", 6);
			FootAudioComponent->Play(0.f);
			break;
	case::EPhysicalSurface::SurfaceType8:
		CLog::Print("Grass", -1, 10.0f, FColor::Blue);
		FootAudioComponent->SetIntParameter("foot", 7);
		FootAudioComponent->Play(0.f);
		break;

		default:
			CLog::Print("NOSurface?", -1, 10.0f, FColor::Blue);
			break;
	}

````
## CPickup 및 ItemInteraction 

**설명**:  Player 와 Test Actor 를 Interface로 연결해서 해당 Actor에 상호작용할시에 해당 actor의 Interaction function을 불러옵니다 ,  

또한 해당 액터와 상호작용하기위한 LineTrace는 해당 Character 의 GetPawnViewLocation 과 GetViewRotation 함수를 사용해서 
카메라 기준이 아닌 해당 캐릭터의 머리 부분에서 LineTrace를 하도록 설정하고 , 
벡터의 내적을 활용해서 캐릭터의 정면 Rotation을 기준으로 180도가 넘어가면 if 문을 넘어가지않게 만들었습니다
 
Post prosses material 에 커스텀 머테리얼을 추가해서 해당 액터가 상호작용 가능할시에 해당 액터의 begin focus 함수와 end focus 함수에 

mesh - set rendercustom depth 라는 렌더링 함수를 사용해서 해당 메시의 테두리 부분을 상호작용할수있게 만들었습니다

### 관련 함수 혹은 코드

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

## InventoryComponent
**설명** : ActorComponent를 활용해서 제작한 Inventory 입니다
FItemAddResult 와 HandleAddItem 을 이용해서 최대한 아이템이 잘못들어가는일이 없도록 설정했으며 AddNewItem 을 통해 추가할때도 해당 객체가 복사된객체인지 아니면 필드에있는 pickup객체인지 확인하고 객체를 복사하여 
Deep copy shallow copy에 대응해서 설계했습니다 , 또한 Stackableitem을 구분해서 인벤토리에 추가하기위해 Item의 ID를 람다식으로 비교하는 FindNextPartialStack으로 인벤토리내의 배열을 순회하여 MaxStackSize가 아닌 개체의 Quanity를 추가해줄수있습니다
```cpp
USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	/*Initialize*/
	FItemAddResult() :
	ActualAmountAdded(0),
	OperationResult(EItemAddResult::IAR_NoItemAdded),
	ResultMessage(FText::GetEmpty())
	{};



	//Actual amount of item that was added to inventory 
	UPROPERTY(BlueprintReadOnly , Category = "Item Add Result")
	int32 ActualAmountAdded;
	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;
	/*ex) "inventory is full, information message that can be passed with the result  "*/
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;



	/* we're gonna call this directly,so we make this with static*/
	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;

		AddedNoneResult.ActualAmountAdded = 0;

		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;

		AddedNoneResult.ResultMessage = ErrorText;

		return AddedNoneResult;
	};

	static FItemAddResult AddedPartial(const int32 PartialAmountAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;

		AddedPartialResult.ActualAmountAdded = PartialAmountAdded;

		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;

		AddedPartialResult.ResultMessage = ErrorText;

		return AddedPartialResult;
	};

	static FItemAddResult AddedAll(const int32 AmountAdded,const FText& Message)
	{
		FItemAddResult AddedAllResult;

		AddedAllResult.ActualAmountAdded = AmountAdded;

		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;

		AddedAllResult.ResultMessage = Message;

		return AddedAllResult;
	};


};

FItemAddResult UCInventoryComponent::HandleAddItem(UItemBase * InputItem)
{
	if (GetOwner())
	{
			const int32 InitialRequestedAddAmount = InputItem->Quanity;

			/*for nonStackable item */
			if (!InputItem->NumbericData.bIsStackable)
			{
					return HandleNonStackableItem(InputItem );
			}

			// handle stackable 
			const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

			if (StackableAmountAdded == InitialRequestedAddAmount)
			{
					// return add all result 
				return FItemAddResult::AddedAll(InitialRequestedAddAmount, FText::Format(
					FText::FromString("Successfully added {0} {1} to the inventory. "),
					InitialRequestedAddAmount,
					InputItem->ItemTextData.Name));
			}

			if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
			{
					// return added partial result 
				return FItemAddResult::AddedPartial(StackableAmountAdded, FText::Format(
					FText::FromString("Partial amount of {0} added to the inventory.Number added = {1} "),
					InputItem->ItemTextData.Name,
					StackableAmountAdded));
			}

			if (StackableAmountAdded <= 0)
			{
					//return added non result 
				return FItemAddResult::AddedNone(FText::Format(
					FText::FromString("Couldn't added {0} to the inventory.No remaining inventory slots , or invalid item."),
					InputItem->ItemTextData.Name));
			}
	


	}

	return FItemAddResult();
}


void UCInventoryComponent::AddNewItem(UItemBase * Item, const int32 AmountToAdd)
{
	/* all the items are pointer , */

	UItemBase* NewItem;

	//some checks 
	if (Item->bIsCopy || Item->bIsPickup)
	{
		/*이미 copy된아이템이거나 레벨에 배치되어있던 item이라면 해당 아이템은 이미 메모리에 참조되어있는 상태 */
		/* if the item is already a copy , or is a world pickup */
		NewItem = Item;
		NewItem->ResetItemFlags();

	}
	else
	{
		// use when splitting or dragging to / from another inventory 
		/*만약 다른경우라면 copy해도된다 */
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuanity(AmountToAdd);

	InventoryContents.Add(NewItem);
	
	/* it could be stack */
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();

}

UItemBase * UCInventoryComponent::FindNextPartialStack(UItemBase * ItemIn) const
{
	/*find by predicate - checks with condition  */
	/* 최대 stack이 아닌 아이템을 람다 함수를 사용해서 찾기 */
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
			InventoryContents.FindByPredicate([&ItemIn](const UItemBase* InventoryItem)
		{
					/*this is not pointer compare , just item ID compare to check is this same item */
			return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack();
			
		}
		))
	{
		return *Result;
	}



	return nullptr;


}
````




## CPlayer

**설명**: DataTable의 Action Data 기반의 Battle Mode 

[Action시연영상](https://www.youtube.com/watch?v=4v_8msfKJ_A)
Unreal Engine C++ 에서 Action DataBase기반의 Action을 가져와서 
해당 직업클래스의 Action을 취할수있게해줍니다 

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



## FeetComponent

**설명**: LineTrace와 기울기를 구해서 IK 구현 
[IK시연영상](https://www.youtube.com/watch?v=-LrjqitSjbw)
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


## Targeting Component 

**설명**: ShpereTrace를 통해 근처의 Pawn을 받아내고 해당 타겟이 범위내에 존재한다면 시야각을 벗어나지않았을때 해당 객체 Targeting, change 를  통해 나를기준으로 가까운거리 , 혹은 Target을 기준으로 가까운거리 로 채인지한다 

[TargetComponent 시연영상](https://www.youtube.com/watch?v=3kYQ9pD75po)


### 관련 함수 혹은 코드
```cpp
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
````
