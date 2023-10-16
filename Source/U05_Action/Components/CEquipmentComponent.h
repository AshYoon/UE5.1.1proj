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

	UFUNCTION(Category = "Equipemt")
	UItemBase* SameTypeValid(UItemBase* ItemIn);

	UFUNCTION(Category = "Equipment")
	FORCEINLINE TArray<UItemBase*> GetEquipmentContents() const { return EquipmentContents; };

	UFUNCTION(Category = "Equipment")
	bool IsTypeValid(EItemType ItemType);


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
	/*TArray �� ���� �����ϴ°� �������� maintenance �κп��� �� ȿ�����ϵ���  */
	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TArray<TObjectPtr<UItemBase>> EquipmentContents;



	//=========================================================================
	//                       FUNCTIONS 
	//=========================================================================

	virtual void BeginPlay() override;

	void SwitchItem(UItemBase* ItemInput , UItemBase* ItemToRemove);

	void ApplyStat();

	void AddNewItem(UItemBase* Item);
		
};
