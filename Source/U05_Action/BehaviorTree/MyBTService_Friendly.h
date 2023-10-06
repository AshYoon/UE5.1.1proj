// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyBTService_Friendly.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API UMyBTService_Friendly : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyBTService_Friendly();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
