// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"


class UItemBase;
class UDragItemVisual;
class UInventoryTooltip;
class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class U05_ACTION_API UInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()


public:
	FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UItemBase* GetItemReferecne() const { return ItemReference; };






protected:
	UPROPERTY(EditDefaultsOnly , Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UInventoryTooltip> ToolTipClass;

	// 보더를 통해서 아이템 quanity or quality를 보여줄수있게설정 
	UPROPERTY(VisibleAnywhere , Category = "Inventory Slot")
	UItemBase* ItemReference;
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UImage* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UTextBlock* ItemQuanity;

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry , const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;


	// when we have stackable stuff you can drop one ,
	//inventory item on another and it will look and see if it's same type ,
	//it'll adjust the stack
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
