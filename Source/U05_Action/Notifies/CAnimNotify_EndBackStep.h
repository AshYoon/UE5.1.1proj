#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndBackStep.generated.h"


UCLASS()
class U05_ACTION_API UCAnimNotify_EndBackStep : public UAnimNotify
{
	GENERATED_BODY()


		FString GetNotifyName_Implementation() const override;

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;



};
