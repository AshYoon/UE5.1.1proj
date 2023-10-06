#include "Inventory/Interactable.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AInteractable::AInteractable()
{

	InteractableHelpText = FString("Press E to interact with item.");




}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInteractable::Interact_Implementation()
{
	GLog->Log("Interact base class : Interact() : You should NOT BE SEEING THIS");

}

