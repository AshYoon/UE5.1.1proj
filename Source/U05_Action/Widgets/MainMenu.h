// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"


class ACPlayer;
/**
 * 
 */
UCLASS()
class U05_ACTION_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY()
		ACPlayer* PlayerCharacter;

	

protected:


	/* widget constructer */
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//when widget detect drop detect use this 
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;


};
