#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CStatusComponent.h"
#include "CBTTaskNode_SetSpeed.generated.h"

UCLASS()
class U05_ACTION_API UCBTTaskNode_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()
		// 부모에있는거 excute 같은거 가져오기 
private:
	UPROPERTY(EditAnywhere)
		ECharacterSpeed SpeedType;

public:
	UCBTTaskNode_SetSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	
};
