// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CEquipmentComponent.h"

// Sets default values for this component's properties
UCEquipmentComponent::UCEquipmentComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
}

// Called when the game starts
void UCEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}






