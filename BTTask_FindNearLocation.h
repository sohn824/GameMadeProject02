// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMADEPROJECT_API UBTTask_FindNearLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindNearLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//ExecuteTask : 태스크를 실행할 때 반드시 실행해야 하는 함수
	//이 함수는 셋 중 하나의 값을 반환해야 한다
	//1. Failed : 태스크 수행 실패
	//2. Succeeded : 태스크 수행 성공
	//3. InProgress : 태스크를 계속 수행 중이다. 실행 결과는 나중에 알려줄 예정이다.
	//비헤이비어 트리의 Sequence 컴포짓은 자신에 속한 태스크를 실패할 때 까지 계속 실행한다
};
