// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class ACPlayer;
struct FInteractableData;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class U05_ACTION_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	ACPlayer* PlayerReference;

	void UpdateWidget(const FInteractableData* InteractableData) const;



	/* doesn't need to be expose to outside */
protected:

	/*use this meta keyword ,we create graphical elements for this widget
	if that graphical part widget deosn't include a text box with this exact
	variable name , you will get a !! COMPILER ERROR !! on the editor 
	this is how we link */
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* NameText;


	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* ActionText;


	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* QuanityText;


	/* for the time of Interaction */
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* KeyPressText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UProgressBar* InteractionProgressBar;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	float CurrnetInteractionDuration;

	// if this function called this will be how to progress bar know how much to fill up
	UFUNCTION(Category = "Interaction Widget | Interactable Data")
	float UpdateInteractionProgress();

	/*                              */
	

	virtual void NativeOnInitialized() override;


	virtual void NativeConstruct() override;


	
};
