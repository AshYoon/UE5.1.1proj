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
	UTextBlock* DamageValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ArmorRating;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UsageText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackSizeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SellValue;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StackWeight;


protected:

	virtual void NativeConstruct();






};
