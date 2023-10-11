// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UItemBase;
class UCInventoryComponent;


/**
 * 
 */
UCLASS()
class U05_ACTION_API UItemDragDropOperation : public UDragDropOperation
{

	GENERATED_BODY()

public:

	UPROPERTY()
	UItemBase* SourceItem;

	UPROPERTY()
	UCInventoryComponent* SourceInventory;




	
};
