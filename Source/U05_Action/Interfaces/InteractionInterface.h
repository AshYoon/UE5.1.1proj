// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"
/*foward declaration , 전방선언*/
class ACPlayer;

// Device -> door , Gate , Interactable Level Design stuff , Toggle - Button or Swich to move SomeThing,  
UENUM()
enum class EInteractableType : uint8
{
	Pickup UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
	Device UMETA(DisplayName = "Device"),
	Toggle UMETA(DisplayName = "Toggle"),
	Container  UMETA(DisplayName = "Container")
};


USTRUCT()
struct FInteractableData
{
	GENERATED_BODY()


		// : -> when constructer runs going to do this stuff , before body of constructer
	FInteractableData() : InteractableType(EInteractableType::Pickup),
	Name(FText::GetEmpty()),
	Action(FText::GetEmpty()),
	Quantity(0),
	InteractableDuration(0.0f)
	{

	};


	UPROPERTY(EditInstanceOnly) // this define what struct is 
	EInteractableType InteractableType;
	/*
	tihs is define what struct is , not Editing directly , we're gonna set our own classes and set == to this Type
	ex) door class , Swich class 
	*/



	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// used only for pickups 
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;
	

	// use for things like valves , doors , etc that require an interaction timer 
	UPROPERTY(EditInstanceOnly)
	float InteractableDuration;



};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 Interface for interaction
 */
class U05_ACTION_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void BeginFocus();
	virtual void EndFocus();

	// make Timer to feed back user , if you want to pickup or interact something ex) Hold 'E' 0.1second 
	virtual void BeginInteract();
	virtual void EndInteract();

	virtual void Interact(ACPlayer* PlayerCharacter);

	FInteractableData InteractableData;



};
