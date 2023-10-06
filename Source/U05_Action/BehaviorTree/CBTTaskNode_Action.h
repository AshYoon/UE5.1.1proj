// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

/**
 * 
 */
UCLASS()
class U05_ACTION_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "AI")
		float Delay = 2.0f;

public:
	UCBTTaskNode_Action();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	float TotalTime = 0.0f;



	
};
