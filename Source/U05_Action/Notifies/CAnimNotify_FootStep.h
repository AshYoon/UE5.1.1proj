// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_FootStep.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API UCAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()


		FString GetNotify_Implementation() const;
		
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
