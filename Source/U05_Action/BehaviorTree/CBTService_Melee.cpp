#include "CBTService_Melee.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	if (state->IsHittedMode())
	{
		behavior->SetHittedMode();

		return;
	}

	ACPlayer* target = behavior->GetTargetPlayer();


	if (target == NULL)
	{
		//TODO: 패트롤 모드

		behavior->SetWaitMode();
		return;
	}
	else
	{
		UCStateComponent* targetState = CHelpers::GetComponent<UCStateComponent>(target);
		if (targetState->IsDeadMode())
		{
			behavior->SetWaitMode();

			return;
		}
	}


	float distance = ai->GetDistanceTo(target);


	if (distance < controller->GetMeleeActionRange())
	{
		behavior->SetActionMode();

		return;
	}

	if (distance < controller->GetSightRadius())
	{
		behavior->SetApproachMode();

		return;
	}
}