#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class U05_ACTION_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	//UPROPERTY(EditDefaultsOnly, Category = "AI")
	//	uint8 TeamID = 1; // 팀 나누는거 

	//private:
	//	UPROPERTY(VisibleDefaultsOnly)
	//		class UCPatrolComponent* Patrol;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; } // 디폴트 behaviortree가 존재함 
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

public:
	ACEnemy_AI();
};
