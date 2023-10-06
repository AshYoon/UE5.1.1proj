// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "CPickup.generated.h"

class UDataTable;
class UItemBase;

UCLASS()
class U05_ACTION_API ACPickup : public AActor , public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================


	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================

	ACPickup();

	/*set in the editor thing are use this , pull everything from the database and you do begin play  */
	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuanity);


	/* in the middle of the game use this fucntion, pull everything from already existing item reference*/
	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuanity);

	FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

	virtual void BeginFocus() override;
	virtual void EndFocus() override;



protected:


	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	/**/
	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;


	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	/*set by the datatable*/
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuanity;

	//set in the editor , ex) door,
	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================
	virtual void BeginPlay() override;


	virtual void Interact(ACPlayer* PlayerCharacter) override;
	void UpdataInteractableData();
	
	void TakePickup(const ACPlayer* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

#endif

};
