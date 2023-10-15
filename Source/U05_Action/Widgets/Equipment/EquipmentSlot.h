// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSlot.generated.h"


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
class U05_ACTION_API UEquipmentSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	FORCEINLINE void SetEquipmentReference(UItemBase* ItemIn) { EquipmentReference = ItemIn; };
	FORCEINLINE UItemBase* GetEquipmentReference() const { return EquipmentReference; };


protected:

	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
		UItemBase* EquipmentReference;

	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot", meta = (BindWidget))
		UBorder* EquipmentBorder;

	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot", meta = (BindWidget))
		UImage* EquipmentIcon;


	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;


	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
