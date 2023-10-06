// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainMenu.h"
#include "Characters/CPlayer.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	

}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();


	// get owner 
	PlayerCharacter = Cast<ACPlayer>(GetOwningPlayerPawn());


}

bool UMainMenu::NativeOnDrop(const FGeometry & InGeometry, const FDragDropEvent & InDragDropEvent, UDragDropOperation * InOperation)
{
	return Super::NativeOnDrop(InGeometry,InDragDropEvent,InOperation);

	// cast operation to item dragdrop , ensure player is valid , call drop item on player 

}
