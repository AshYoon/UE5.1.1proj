#include "Components/CMontagesComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"




UCMontagesComponent::UCMontagesComponent()
{




}



void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();


	CheckNull(DataTable);

	TArray< FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	//for (const FMontageData* data : datas)
	//{
	//   if (!!data)// 데이터가있다면 
	//      CLog::Print(data->AnimMontage->GetPathName()); // 경로 출력 
	//}

	for (int32 i = 0; i < (int32)EStateType::Max; i++) // 전체 크기 포문돌려서 
	{
		for (FMontageData* data : datas) 
		{
			if ((EStateType)i == data->Type) // 숫자를 enum으로 바꿔서 Type이랑 맞으면 데이터 넣어주기
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)
	
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackstep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InState)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InState];
	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);// 우리가 데이터 테이블에 세팅한 옵션 
	}
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}


