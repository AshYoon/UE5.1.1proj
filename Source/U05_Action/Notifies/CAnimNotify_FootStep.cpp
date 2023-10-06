// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_FootStep.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_FootStep::GetNotify_Implementation() const
{
	return "FootStep";
}

void UCAnimNotify_FootStep::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->FootStep();
	
	



	
	
}
