// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Equipment/EquipmentSlot.h"

void UEquipmentSlot::NativeOnInitialized()
{
}

void UEquipmentSlot::NativeConstruct()
{
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// take widget just return reference of the unerlying widget 
		/*if we hold or not move it , this won't fire */
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}


	// submenu on right click will happen here 

	return Reply.Unhandled();
}

void UEquipmentSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UEquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UEquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
