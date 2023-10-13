// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CInventoryComponent.h"
#include "CEquipmentComponent.generated.h"

class ACPlayer;
class UItemBase;



DECLARE_MULTICAST_DELEGATE(FOnEquipmentUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()



public:
	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================

	FOnEquipmentUpdated OnEquipmentUpdated;


	


	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================
	UCEquipmentComponent();




	UFUNCTION(Category = "Equipment")
	void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);

	UFUNCTION(Category = "Equipment")
	FItemAddResult HandleAddItem(UItemBase* InputItem);

	UFUNCTION(Category = "Equipment")
	UItemBase* FindMathingItem(UItemBase* ItemIn) const;

	UFUNCTION(Category = "Equipment")
	FItemAddResult FindNextItemByItemType(UItemBase* ItemIn);


private:

	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================



	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================




protected:

	//=========================================================================
	//                       PROPERTIES & VARIABLES
	//=========================================================================
	/*TArray 를 통해 관리하는게 가독성과 maintenance 부분에서 더 효율적일듯함  */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>> EquipmentContents;



	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================

	virtual void BeginPlay() override;

	void SwitchItem(UItemBase* ItemInput , UItemBase* ItemToRemove);
		
};
