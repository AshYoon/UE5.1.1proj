#include "CAIController.h"
#include "Global.h"
#include "CEnemy_AI.h"
#include "CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	//CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	
	MyBlackboard = CHelpers::CreateActorComponent<UBlackboardComponent>(this, FName("Blackboard"));
	
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 90;
	Sight->SetMaxAge(2);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();




}


void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);



	//SetGenericTeamId(OwnerEnemy->GetTeamID());// 여기로 Team id를 가져옴 
	TeamID = FGenericTeamId(OwnerEnemy->ID); // ai team id를 owner의 id로 세팅 




	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);


	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, MyBlackboard);
	//this->UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);

	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());

}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

ETeamAttitude::Type ACAIController::GetTeamAttitudeTowards(const AActor & other) const
{
	// if pawn 
	const APawn* OtherPawn = Cast<APawn>(&other);
	if (OtherPawn == nullptr)
	{
		return ETeamAttitude::Neutral;
	} // pawn 이 아닐경우 중립 
	CLog::Print("dsd", 1.0f);

	// 해당 액터가 genericTeamAgentinerface로 implement되었는지확인 ( 같은 interface로 묶였는지확인 ) 
	// 만약 같은 interface ( IGenericTeamAgentInterface) 로 묶여있다면 캐스팅되고 아니면 neutral을 반환 
	auto PlayerTI = Cast<IGenericTeamAgentInterface>(&other); // playerTeamId cast로 team id 받아오기 
	class IGenericTeamAgentInterface* botTI = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (botTI == nullptr && PlayerTI == nullptr)
	{
		return ETeamAttitude::Neutral;
	}

	//get actor team id 
	FGenericTeamId OtherActorTeamid = NULL;
	if (botTI != nullptr)
	{
		OtherActorTeamid = botTI->GetGenericTeamId();
	}
	else if (PlayerTI != nullptr)
	{
		OtherActorTeamid = PlayerTI->GetGenericTeamId();
	}

	// check hostile , 적인지아닌지확인 
	FGenericTeamId ThisId = GetGenericTeamId();
	if (OtherActorTeamid == 8)
	{
		return ETeamAttitude::Neutral;
	}
	else if (OtherActorTeamid == ThisId)
	{
		//Blackboard->SetValueAsClass("Friendly");
		return ETeamAttitude::Friendly;
		
	}
	else
	{
		return ETeamAttitude::Hostile;
	}


}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bDrawDebug);

	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= AdjustCircleHeight;
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
	DrawDebugCircle(GetWorld(), center, MeleeActionRange, 300, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);




}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	//TArray<AActor*> Threats;
	//PerceptionComponent->GetHostileActors(Threats);
	//if (Threats.Num() <= 0)
	//{
	//	return;
	//}




	APawn* Threat = NULL;


	for (AActor* actor : actors)
	{
		Threat = Cast<APawn>(actor);

		if (!!Threat)
		{

			break;
		}
			
	}

	Blackboard->SetValueAsObject("Player", Threat);




}
