// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Action.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	bNotifyTick = true;

	NodeName = "Action"; // set this node name 
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory); // super로 최상위 class 의 기능 부르기 
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner()); // same func blueprints get owner
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());// get pawn
	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(ai);

	TotalTime = 0.0f;
	action->DoAction();


	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	TotalTime += DeltaSeconds;

	if (state->IsIdleMode() && TotalTime > Delay)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);


}
