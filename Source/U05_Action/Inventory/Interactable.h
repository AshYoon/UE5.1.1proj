// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class U05_ACTION_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame



	UFUNCTION(BlueprintNativeEvent)
		void Interact();

	virtual void Interact_Implementation();


public:
	// all interactables will have static mesh 
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		class UStaticMeshComponent* InteractableMesh;


	// all interactables will have help text
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		FString InteractableHelpText;




	
};
