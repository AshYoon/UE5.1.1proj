// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UInventoryItemSlot;
class UCInventoryComponent;
class ACPlayer;
class UTextBlock;
class UWrapBox;

/*
 it's just a pure array , it's not going to be a slot based inventory 
 */
UCLASS()
class U05_ACTION_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()


public:
	//bind this update with delegate 
	UFUNCTION()
	void RefreshInventory(); 


	/*make editor declare in C++ medatory to be in the bp or else it will not compile */
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryWarpBox;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	ACPlayer* PlayerCharacter;

	UPROPERTY()
	UCInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InvnetorySlotClass;



protected:
	//set weight capacity info 
	void SetInfoText() const;

	virtual void NativeOnInitialized() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
};
