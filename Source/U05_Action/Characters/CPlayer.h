#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CInventoryComponent.h"
#include "Components/CEquipmentComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Characters/ICharacter.h"
#include "Inventory/Interactable.h"
#include "Inventory/Pickup.h"
#include "GenericTeamAgentInterface.h"
#include "Interfaces/InteractionInterface.h"
#include "CPlayer.generated.h"

/*
InteractionDataStruct - info of Interaction 
*/
class ACHUD;
class UCInventoryComponent;


USTRUCT()
struct FInteractionData
{
	GENERATED_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	// contain current Interactable
	UPROPERTY()
	AActor* CurrentInteractable;


	// how often you want to check interactable , not need to shoot linetrace every single time 
	UPROPERTY()
	float LastInteractionCheckTime;

};


// state에는 enum이 있다 , enum은 전방선언 이안된다 
// enum문에 접근해야하니깐 헤더에 넣어줘야한다 
// 상속구조 추가하는 방법 Check 
UCLASS()
class U05_ACTION_API ACPlayer : public ACharacter,public IICharacter , public IGenericTeamAgentInterface 
{
	
	GENERATED_BODY()


		//=========================================================================
		//                       PROPERTIES & VARIABLES
		//=========================================================================



private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_ActionList> ActionListClass;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		int32 ID = 255;


	//virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; } //team id 세팅 
	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }

private:

	// 필요한건 컴포넌트에 추가하고 그 추가한대상이 Player 

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	// 타겟 잡기위한 
	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;



	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UCFeetComponent* Feet;


	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;


	

	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;



	UPROPERTY(EditAnywhere, Category = "Equipment")
		class UCEquipmentComponent* Equipment;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Audio",meta = (AllowPrivateAccess = "true") )
		class USoundCue* FootSoundCue;

	UAudioComponent* FootAudioComponent;

public:
	FORCEINLINE class UCUserWidget_ActionList* GetActionList() { return ActionList; };
	FORCEINLINE UCInventoryComponent* GetInventory() const { return PlayerInventory; };

	void UpdateInteractionWidget() const;

	// the players help Text
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
	FString HelpText;

	//The mount of gold the player has 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "HUD")
	int32 Gold;

	//update the coins
	// param amount this is the mount to update the coins by , and can be positive or negative

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UpdateGold(int32 Amount);

	//Adds an item to inventory
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(APickup* Item);

	//Gets the thumbnail for a given inventroy slot 
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	UTexture2D* GetThumbnailAtInventorySolt(int32 Slot);

	//gets the items name for a given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	FString GetItemNameAtInventorySlot(int32 Slot);

	//Uses the item at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void UseItemAtInventorySlot(int32 Slot);

private:

		//The Players Reach ( is the Reach ,help Thought reach form player 
		float Reach;



		// Toggle this inventory
		void ToggleInventory();

		//Interacts with the current interactable provided there is one 
	//	void Interact();

		// checks for interactable items directly in front of the player using line trace ,called on per tick basis
		void CheckForInteractables();



		//The interactable player is currently looking for
		AInteractable* CurrentInteractable;

		// the players inventory , represented as a Tarray of pickup objects
		UPROPERTY(EditAnywhere)
			TArray<APickup*> Inventory;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;
	// 색은 게임이 시작됬을때 바꿔주면된다 , Player가어떤 상태인지 알려줄수있는 
	// TypeColor를 해볼꺼다 
private:
	class UCUserWidget_ActionList* ActionList;


protected:
	// what we hit with linetrace
	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UCInventoryComponent* PlayerInventory;

	float InteractionCheckFrequency;
	
	
	float InteractionCheckDistance;

	FTimerHandle TimerHandle_Interaction;

	FInteractionData InteractionData;

	UPROPERTY()
	ACHUD* HUD;
	

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
//		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;
	
//=========================================================================
//                       FUNCTIONS 
//=========================================================================

	/*Interaction Functions */


		void PerformInteractionCheck();


		void FoundInteractable(AActor* NewInteractable);


		void NoInteractableFound();


		void BeginInteract();


		void EndInteract();


		void Interact(); // Action of Interacting 

		void ToggleMenu();
public:

	ACPlayer();

protected:

	virtual void BeginPlay() override;


	FGenericTeamId TeamID; //TeamID변수 
private:
	/* Character Movement functions */
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);
	void OnZoom(float InAxis);
	

private:
	void OnAvoid();


public:	

	virtual void Tick(float DeltaTime) override;


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/* not need to change read only function -> make with const */
	
	/* more use for interaction 'Time'   */
	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); };
	// ForTimer , Delay 
	FTimerHandle GravityTimerHandle;
	float GravityTime = 3.f;


private:
	/* Character Actions Function */


	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	// add dynamic으로 
private:
	//Actions 
	void Begin_Roll();
	void Begin_Backstep();

public:
	void FootStep();
	void End_Roll();
	void End_Backstep();
private:
	UFUNCTION()
	void OnOneHand();
	UFUNCTION()
	void OnTwoHand();
	UFUNCTION()
	void OnFist();
	UFUNCTION()
	void OnWarp();
	UFUNCTION()
	void OnFireStorm();
	UFUNCTION()
	void OnIceBall();

	void OnAim();
	void OffAim();
	void OnDoAction();
	void OnViewActionList();
	void OffViewActionList();
	void OnTarget();
	void OnTargetLeft();
	void OnTargetRight();



public:
	virtual void ChangeColor(FLinearColor InColor) override;
	





};
