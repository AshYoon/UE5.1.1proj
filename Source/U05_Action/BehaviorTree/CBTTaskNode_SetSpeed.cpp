#include "BehaviorTree/CBTTaskNode_SetSpeed.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"



UCBTTaskNode_SetSpeed::UCBTTaskNode_SetSpeed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(ai);

	status->SetSpeed(SpeedType);
	
	return EBTNodeResult::Succeeded;


}