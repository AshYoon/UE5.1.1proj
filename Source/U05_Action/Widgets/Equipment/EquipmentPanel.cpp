// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Equipment/EquipmentPanel.h"
#include "Widgets/Equipment/EquipmentSlot.h"
#include "Characters/CPlayer.h"


#include "Components/WrapBox.h"
#include "Components/TextBlock.h"

void UEquipmentPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<ACPlayer>(GetOwningPlayerPawn());

	if (PlayerCharacter)
	{
		EquipmentReference = PlayerCharacter->GetEquipment();
		if (EquipmentReference)
		{
			EquipmentReference->OnEquipmentUpdated.AddUObject(this, &UEquipmentPanel::RefreshEquipment);

		}
	}


}

bool UEquipmentPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UEquipmentPanel::RefreshEquipment()
{
	if (EquipmentReference && EquipmentSlotClass)
	{
		EquipmentWarpBox->ClearChildren();
		
		for (UItemBase* const& EquipmentItem : EquipmentReference->GetEquipmentContents())
		{
			UEquipmentSlot* EquipmentSlot = CreateWidget<UEquipmentSlot>(this, EquipmentSlotClass);

			EquipmentSlot->SetEquipmentReference(EquipmentItem);
			EquipmentWarpBox->AddChildToWrapBox(EquipmentSlot);
		}
	}
}
