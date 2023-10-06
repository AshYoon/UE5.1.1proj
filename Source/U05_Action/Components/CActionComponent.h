#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"


UENUM(BlueprintType)
enum class EActionType : uint8
{
	Unarmed, Fist, OneHand, TwoHand, Warp, FireStorm, IceBall, Max,
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U05_ACTION_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
		class UCActionData* DataAssets[(int32)EActionType::Max];
	//이름수정 
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UCAction* GetCurrent() { return Datas[(int32)Type]; }
	// ActionData가 아니라 Action으로 리턴해줘야한다.

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFistMode() { return Type == EActionType::Fist; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFireStormMode() { return Type == EActionType::FireStorm; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIceBallMode() { return Type == EActionType::IceBall; }


	// 체크하기위해 인라인함수 생성 



public:	

	UCActionComponent();

public:
	UFUNCTION(BlueprintCallable)
		void SetUnarmedMode();

	UFUNCTION(BlueprintCallable)
		void SetTwoHandMode();



	void SetFistMode();
	void SetOneHandMode(); 

	void SetWarpMode();
	void SetFireStormMode();
	void SetIceBallMode();


	void OffAllCollision();
public:
	void DoAction();
	void DoAim();
	void UndoAim();
private:
	void SetAimMode(bool InAim);
protected:

	virtual void BeginPlay() override;


public:
	// private이면 외부연결안되니깐 public으로 
	UPROPERTY(BlueprintAssignable)
		FActionTypeChanged OnActionTypeChanged;
private:
	void SetMode(EActionType InType);


	void ChangeType(EActionType InNewType);
		
private:

	// 타입을 위한 변수 생성 
	EActionType Type;
	class ACEquipment* Equipment;
	
	//ActionType에 대한 Data

	class UCAction* Datas[(int32)EActionType::Max];


};
