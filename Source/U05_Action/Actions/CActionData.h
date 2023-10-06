#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Actions/CAction.h"
#include "CActionData.generated.h"



UCLASS()
class U05_ACTION_API UCActionData : public UDataAsset
{
	GENERATED_BODY()
	// ����ü���鲨�� 
	// Equipment���õȰ� 
	// Action ���õȰ� 

public:




public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	
 	//FEquipmentData undefined 
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FLinearColor EquipmentColor;

	   UPROPERTY(BlueprintReadOnly, EditAnywhere)
      TSubclassOf<class ACDoAction> DoActionClass;

       UPROPERTY(BlueprintReadOnly, EditAnywhere)
      TArray<FDoActionData> DoActionDatas;
	   // undefiend type FDoActionData 




public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class UCAction** OutAction);
	// UCAction�� 


private:
	FString GetLableName(class ACharacter* InOwnerCharacter, FString InName);
	// �� ������ Warp�� IF���� �ȵ��´� 



};
