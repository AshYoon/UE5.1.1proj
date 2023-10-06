#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnequipmentDelegate);


UCLASS()
class U05_ACTION_API ACEquipment : public AActor
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetData(FEquipmentData InData) { Data = InData; }
	FORCEINLINE void SetColor(FLinearColor InColor) { Color = InColor; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	// 주소만 리턴해서 확인만 가능하고 const라서 변경안됨 
public:
	// Equip 델리게이트 
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

public:	
	
	ACEquipment();
public:
	//장착 명령
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//장착 개시
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//장착 완료
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();


	//해제 명령
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();



protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

private:
	FEquipmentData Data;

	FLinearColor Color;

protected:

	virtual void BeginPlay() override;
	bool bEquipped;
	// 장착여부 확인 변수 

};
