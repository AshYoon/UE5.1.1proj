// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Pickup.h"
#include "Characters/CPlayer.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
//#include "Inventory.h"


APickup::APickup()
{
	//set up the mesh for the pickup , and set the item name , help text and item value 
	//InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickMesh"));

	//CHelpers::CreateActorComponent<UCInventoryComponent>(this, &PickUpInventroy, "Inventory");



	//// Only use Collsion Box and Particle System 
	//CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpBox"));
	//CollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	//ItemParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PickUpParticle"));


	////rule for attach particle to collision box 
	//const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	//ItemParticle->AttachToComponent(CollisionBox, AttachmentTransformRules);



	////ItemParticle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	////CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	////CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//



	//


	////InteractableMesh->SetSimulatePhysics(false);

	//ItemName = FString("Enter an item name here .. ");




	//InteractableHelpText = FString("Press F to pick item up.");

	//Value = 0;

	////
	//ItemType = EItemType::ETC;


	

}

void APickup::Interact_Implementation()
{


	//// all interactive item will use this function 

	//ACPlayer* Character = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
	////put code here that places the item int othe characters inventory 


	//// if Character inventory is full Item will not PickUped 
	//if (Character->AddItemToInventory(this))
	//{
	//	OnPickedUp();
	//}






}

void APickup::BeginPlay()
{
	Super::BeginPlay();
	// (ItemName) : Press F to pick up.
	//InteractableHelpText = FString::Printf(TEXT("%s "), *ItemName);
	//InteractableMesh->SetSimulatePhysics(true);

}

void APickup::Use_Implementation()
{

	GLog->Log("Use() from base pickup class : YOU SHOULD NOT BE SEEING THIS");


}

void APickup::OnPickedUp()
{
	//ItemParticle->SetVisibility(false);
	//CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//InteractableMesh->SetVisibility(false);
	//InteractableMesh->SetSimulatePhysics(false);
	//InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

}
