// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryTooltip.generated.h"


class UInventoryItemSlot;
class UTextBlock;
/**
 * 
 */
UCLASS()
class U05_ACTION_API UInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()
	


public:
	UInventoryItemSlot* InventorySlotBeingHobered;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemType;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModifierType;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ModifierValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UsageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxStackSize;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxStackSizeText;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* SellValue;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackWeightText;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackWeight;


protected:

	virtual void NativeConstruct() override;






};
