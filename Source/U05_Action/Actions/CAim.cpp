#include "Actions/CAim.h"
#include "Global.h"
#include "CHUD.h" //크로스헤어 만들기위한 헤더 
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"







UCAim::UCAim()
{
	// 이름 똑같은지 확인 
	//CurveFloat'/Game/Actions/CAim.CAim'
	// 커브랑 타임라인이용해서 보간 .

	CHelpers::GetAssetDynamic<UCurveFloat>(&Curve, "CurveFloat'/Game/Actions/CAim.CAim'");
	// Helper오류는 대부분 레퍼런스 경로에서 문제생길경우인게 많다 

}


void UCAim::BeginPlay(ACharacter * InCharacter)
{
	OwnerCharacter = InCharacter;

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	HUD = OwnerCharacter->GetWorld()->GetFirstPlayerController()->GetHUD<ACHUD>();


	TimelineFloat.BindUFunction(this, "Zooming");

	Timeline.AddInterpFloat(Curve, TimelineFloat);
	Timeline.SetPlayRate(200); // 타임라인 속도 

	



}



void UCAim::Tick(float DeltaTime)
{

	Timeline.TickTimeline(DeltaTime);
	
}



void UCAim::OnZoom()
{
	CLog::Print("OnZoom");
	//CheckFalse(IsAvaliable());
	CheckTrue(bInZoom);

	bInZoom = true;

	SpringArm->TargetArmLength = 100.0f;
	SpringArm->SocketOffset = FVector(0, 30, 10);
	SpringArm->bEnableCameraLag = false;

	//Camera->FieldOfView = 45;
	//
	Timeline.PlayFromStart(); // 시작부터

	HUD->SetDrawMode();

}

void UCAim::OffZoom()
{
	CLog::Print("OffZoom");

	CheckFalse(bInZoom);

	bInZoom = false;

	SpringArm->TargetArmLength = 250.0f;
	SpringArm->SocketOffset = FVector(0, 0, 0);
	SpringArm->bEnableCameraLag = true;

	//Camera->FieldOfView = 90;

	HUD->SetUndrawMode();

	Timeline.ReverseFromEnd();
}




void UCAim::Zooming(float Output)
{

	CLog::Print(Output, 0);

	Camera->FieldOfView = Output;
}