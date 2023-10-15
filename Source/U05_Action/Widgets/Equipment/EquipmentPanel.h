// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentPanel.generated.h"


class UEquipmentSlot;
class UCEquipmentComponent;
class ACPlayer;
class UTextBlock;
class UWrapBox;

/**
 * 
 */
UCLASS()
class U05_ACTION_API UEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()


public:
	//bind this update with delegate 
	UFUNCTION()
	void RefreshEquipment();


	/*make editor declare in C++ medatory to be in the bp or else it will not compile */
	UPROPERTY(meta = (BindWidget))
		UWrapBox* EquipmentWarpBox;

	UPROPERTY()
		ACPlayer* PlayerCharacter;

	UPROPERTY()
		UCEquipmentComponent* EquipmentReference;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UEquipmentSlot> EquipmentSlotClass;

protected:

	virtual void NativeOnInitialized() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


};
