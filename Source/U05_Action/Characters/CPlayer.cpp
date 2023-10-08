#include "CPlayer.h"
#include "CGameMode.h"
#include "Global.h"
/*Components */
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/CActionComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CInventoryComponent.h"
#include "Components/CFeetComponent.h"

#include "Widgets/CUserWidget_ActionList.h"
/* Engine Stuff */
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h" // 카메라 쉐이크 쓸때 필요 
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "CHUD.h"


// 루트폴더 잡혀있으니깐 경로 모두 안적어도된다 





ACPlayer::ACPlayer()
{
 	PrimaryActorTick.bCanEverTick = true;

	// 인텔리 센스가 오래 걸린다 , 


	// 카메라 컴포넌트 스프링암 컴포넌트 
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target" );

	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");
	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");

	//Load Sound cue object
	//CHelpers::GetAsset<USoundCue>(&FootSound, "SoundCue'/Game/Sound/FootStep.FootStep'");
	static ConstructorHelpers::FObjectFinder<USoundCue> FootSoundObject(TEXT("SoundCue'/Game/Sound/FootStep.FootStep'"));
	if (FootSoundObject.Succeeded())
	{
		FootSoundCue = FootSoundObject.Object;

		FootAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootAudioComponent"));
		FootAudioComponent->SetupAttachment(RootComponent);


	}

	bUseControllerRotationYaw = false;


	// 메시 
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;

	//Skeleton'/Game/Character/Mesh/Skel_Mannequin.Skel_Mannequin' 항상 경로 체크 
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	//AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer'
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	// 스프링 암 
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	// 캐릭터 무브먼트 
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//WB_ActionList
	CHelpers::GetClass<UCUserWidget_ActionList>(&ActionListClass, "WidgetBlueprint'/Game/WIdgets/WB_ActionList.WB_ActionList_C'");


	//Reach for CheckForInteractables();
	//Reach 거리에 따라서 pickup이 가능 
	//Reach = 500.0f;


	// 16ms is one fream , don't need to setting lower than that , won't have any point 
	//100ms is fine 
	InteractionCheckFrequency = 0.1;
	InteractionCheckDistance = 225.0f;

	// it's public so we can override without getter or setter (not working ) 
	//BaseEyeHeight = 150.0f;
	BaseEyeHeight = 74.0f;
	//RecalculateBaseEyeHeight();

	PlayerInventory = CreateDefaultSubobject<UCInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotsCapacity(20);
	PlayerInventory->SetWeightCapacity(50.f);
		
}


void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Player의 상태를 관리 AddDynamic으로 델리게이트
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);


	UMaterialInstanceConstant* body; // 바디 색 
	UMaterialInstanceConstant* logo; // 로고 색 

	// 경로 
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'");
	//MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	LogoMaterial = UMaterialInstanceDynamic::Create(logo, this);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	Action->SetUnarmedMode();

	//ActionList = CreateWidget<UCUserWidget_ActionList, APlayerController>(GetController<APlayerController>(), ActionListClass);
	APlayerController* PlayerController = GetController<APlayerController>();
	ActionList = CreateWidget<UCUserWidget_ActionList, APlayerController*>(PlayerController, ActionListClass);
	ActionList->AddToViewport();
	ActionList->SetVisibility(ESlateVisibility::Hidden);
	// 키를 눌렀을때 AddToViewPort로 화면에 생성 


	// 눌러서 바로 넘어간다 
	ActionList->GetData(0).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFist);
	ActionList->GetData(1).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnOneHand);
	ActionList->GetData(2).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnTwoHand);
	ActionList->GetData(3).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnWarp);
	ActionList->GetData(4).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnFireStorm);
	ActionList->GetData(5).OnUserWidget_ActionItem_Clicked.AddDynamic(this, &ACPlayer::OnIceBall);

	if (FootAudioComponent && FootSoundCue)
	{
		FootAudioComponent->SetSound(FootSoundCue);
	}


	//Inventory 
	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

	/*Get my HUD*/
	HUD = Cast<ACHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());




}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

	 
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}


	//CheckForInteractables();


}

/*Create LineTrace , CheckFortheInterface,SetTarget Interactable */
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

void ACPlayer::FoundInteractable(AActor * NewInteractable)
{
	// check for previous interactable 
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		/*if we have one , we gonna setup target interactable*/
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->BeginFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	/*must be passing it by reference*/
	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();



}

void ACPlayer::NoInteractableFound()
{
	// if we can't find interactable , clear  the world timer for interaction timerhandle
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			/*there is time for destroy before this, it would destroy pickup , before this prosses is gone  */
			//remove for crush , IsValid will check it 
			TargetInteractable->EndFocus();

		}
		// hide interaction widget on the HUD
		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}




}

void ACPlayer::BeginInteract()
{
	/*origin of interactions */
	/*doublecheck ? , quick check to verify nothing has change with the interactable state since beginning interaction*/
	// just for fail safe ! , 
	PerformInteractionCheck();

	// always check this 
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			/* 0.1 s , is really fast ,if interaction time is really short it could be pointless */
			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractableDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				/*if valid delay above 0.1s which is what we set start timer , with interactableDuration */
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&ACPlayer::Interact,
					TargetInteractable->InteractableData.InteractableDuration,
					false);
			}
		}
	}


}

void ACPlayer::EndInteract()
{
	/* doesn't need to check interact*/
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}




}

void ACPlayer::Interact()
{
	//CheckForInteractables();
	//CLog::Print("Interact", -1, 10.0f);
	// if we find interactable do interactable implementation
	//if (CurrentInteractable != nullptr)
	//{
	//	CurrentInteractable->Interact_Implementation();
	//}

	/*new version of interact with Interactable Duration and WorldTimer */
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);



	if (IsValid(TargetInteractable.GetObject()))
	{
		/*this character call the interact */
		TargetInteractable->Interact(this);
	}





}





void ACPlayer::UpdateInteractionWidget() const
{
	/*do a check ,interaction segment */
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}






//=========================================================================
//                      BINDING KEY 
//=========================================================================

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);




	/////Axis 

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	/// my action key = F 
	PlayerInputComponent->BindAction("InterActive", EInputEvent::IE_Pressed,this, &ACPlayer::BeginInteract);
	PlayerInputComponent->BindAction("InterActive", EInputEvent::IE_Released, this, &ACPlayer::EndInteract);
	// Toggle Inventory key = i
	PlayerInputComponent->BindAction("ToggleInventory", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleInventory);

	PlayerInputComponent->BindAction("ToggleMenu", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleMenu);



	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);
	PlayerInputComponent->BindAction("TwoHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	// warp change key to 5 
	PlayerInputComponent->BindAction("OnWarp", EInputEvent::IE_Pressed, this, &ACPlayer::OnWarp);
	PlayerInputComponent->BindAction("FireStorm", EInputEvent::IE_Pressed, this, &ACPlayer::OnFireStorm);
	PlayerInputComponent->BindAction("IceBall", EInputEvent::IE_Pressed, this, &ACPlayer::OnIceBall);
	
	PlayerInputComponent->BindAction("TargetLeft", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetLeft);
	PlayerInputComponent->BindAction("TargetRight", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetRight);


	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, this, &ACPlayer::OnTarget);

	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);


	// key K 로 변경 change to key K 
	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewActionList);
	PlayerInputComponent->BindAction("ViewActionList", EInputEvent::IE_Released, this, &ACPlayer::OffViewActionList);
	
	
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);


}

// 음수 양수에 따라서 좌우 앞뒤가 생기니깐 그걸로 판단 
void ACPlayer::OnMoveForward(float InAxis)
{
	// Move Forward 
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0); // 
	FVector direction = FQuat(rotator).GetForwardVector(); // 항상 전방벡터 방향 

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();




	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::ToggleMenu() 
{
	HUD->ToggleMenu();


}


void ACPlayer::OnZoom(float InAxis)
{
	SpringArm->TargetArmLength += (1000.0f * InAxis * GetWorld()->GetDeltaSeconds());
	// 자연스럽게 움직이니깐 우리가 보간할 필요가없다 

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 50.0f, 500.0f);
	// 최대값 설정 해야하니깐 Clamp를 이용 

}


void ACPlayer::OnAvoid()
{
	CheckFalse(Status->CanMove());  // status canmove 가져와서 checkfalse 아니면 즉 canmove가 가능하면 
	CheckFalse(State->IsIdleMode()); // Idle 상태일때 

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f) // 들어온 값체크 
	{
		State->SetBackstepMode();

		return;
	}

	State->SetRollMode();
}


void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	// State에 따라서 우리가 거기에 맞게 실행해주면 되는거다 
	switch (InNewType)
	{
	case EStateType::Roll:  Begin_Roll(); break;
	case EStateType::Backstep: Begin_Backstep(); break;
	}
}

void ACPlayer::Begin_Roll()
{
	//Roll은 Start지점에서 해당방향으로 회전방향 잡아주고 그방향으로 구르기만 하면된다 
	// Roll은 Yaw가 풀려도된다 자유롭게 회피해야하니깐 
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();
}

void ACPlayer::Begin_Backstep()
{
	// 방향을 유지한채로 뒤로 가는거다 
	// 게속 바라보는대로 

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackstep();

	// 이렇게 되면 Yaw가 섞이게되니깐 End에서 다시 돌려줘야한다 

}

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




}

void ACPlayer::End_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	State->SetIdleMode();


}

void ACPlayer::End_Backstep()
{
	// 일단 Add 는 Action 만들기전이니깐 
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	ChangeColor(FColor::Green);
	// 캐릭터 방향은 반대로 
	State->SetIdleMode();

}

void ACPlayer::OnOneHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetOneHandMode();
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}


void ACPlayer::ChangeColor(FLinearColor InColor)
{

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);


}

void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());

	Action->SetTwoHandMode();


}

void ACPlayer::OnFist()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFistMode();
}

void ACPlayer::OnWarp()
{
	CheckFalse(State->IsIdleMode());

	Action->SetWarpMode();
}

void ACPlayer::OnFireStorm()
{
	CheckFalse(State->IsIdleMode());

	Action->SetFireStormMode();
}

void ACPlayer::OnIceBall()
{
	CheckFalse(State->IsIdleMode());

	Action->SetIceBallMode();
}

void ACPlayer::OnAim()
{
	Action->DoAim();
}

void ACPlayer::OffAim()
{
	Action->UndoAim();
}





void ACPlayer::OnViewActionList()
{
	CheckFalse(State->IsIdleMode()); //Idle모드에서만 띄우기 

	ActionList->SetVisibility(ESlateVisibility::Visible);

	GetController<APlayerController>()->bShowMouseCursor = true; // UI 떳으니깐 마우스 커서 
	GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI()); // UI 모드 랑 Game모드 랑 같이 

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f); // GamePlay 딜레이시켜주기 
}

void ACPlayer::OffViewActionList()
{
	ActionList->SetVisibility(ESlateVisibility::Hidden);

	GetController<APlayerController>()->bShowMouseCursor = false;
	GetController<APlayerController>()->SetInputMode(FInputModeGameOnly()); // Game만 

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f); // 시간 되돌려주기 
}


void ACPlayer::ToggleInventory()
{
	//Code to inventory
	//check players hud state , if inventroy is opne then close,otherwise open inventory
	ACGameMode* GameMode = Cast<ACGameMode>(GetWorld()->GetAuthGameMode());
	//gamemode enum hudstate change ui 
	if (GameMode->GetHUDState() == GameMode->HS_Ingame)
	{
		GameMode->ChangeHUDState(GameMode->HS_Inventory);
	}
	else
	{
		GameMode->ChangeHUDState(GameMode->HS_Ingame);
	}
	


}




void ACPlayer::CheckForInteractables()
{
	//
	


	// TO linetrace , get the start and end traces
	FVector StartTrace = Camera->GetComponentLocation();

	FVector EndTrace = (Camera->GetForwardVector() * Reach) + StartTrace;

	//Declare a hit result to stroe the raycast hit in
	FHitResult HitResult;

	// Initialize the query params - ignore the actor , for example my self
	// Ignore 할 Actor 
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	
	//Cast the line trace ,, ECCWorldDynamic ? 
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);
	//GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Interactable, CQP);
	DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, 5.f);

	AInteractable* PotentialInteractable = Cast<AInteractable>(HitResult.GetActor()); //cast to interactable actor

	if (PotentialInteractable == NULL)
	{
		//CLog::Print("Nocheckforinteractables", -1, 10.0f);
		HelpText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
	else
	{
		//CLog::Print("checkforinteractables", -1, 10.0f);
		CurrentInteractable = PotentialInteractable;
		HelpText = PotentialInteractable->InteractableHelpText;

		//after 3.f second HelpText = " " 
		GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{

			HelpText = FString("");

			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);


	}

}

void ACPlayer::OnTarget()
{

	Target->ToggleTarget();

}


void ACPlayer::OnTargetLeft()
{
	Target->ChangeTargetLeft();
}

void ACPlayer::OnTargetRight()
{
	Target->ChangeTargetRight();
}



// inventory stuff


void ACPlayer::UpdateGold(int32 Amount)
{
	Gold = Gold + Amount;
}

bool ACPlayer::AddItemToInventory(APickup * Item)
{
	if (Item != NULL)
	{
		const int32 AvailableSlot = Inventory.Find(nullptr); // Find first slot with nullptr in it
		if (AvailableSlot != INDEX_NONE)
		{
			Inventory[AvailableSlot] = Item;
			return true;

		}
		else
		{
			// show error message if inventory has not AvailableSlot, 추후에 게임 ui 로 log change 
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You can't carry any more items!"));
			return false;
		}
	}
	else return false;
}

UTexture2D * ACPlayer::GetThumbnailAtInventorySolt(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupThumbnail;
	}
	else return nullptr;


}

FString ACPlayer::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->ItemName;
	}
	return FString("None");

}

void ACPlayer::UseItemAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		Inventory[Slot]->Use_Implementation();
		Inventory[Slot] = NULL;  // delete item once use 
	}




}
