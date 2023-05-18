// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "ChasePlayer_BTTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MORTALMOTORPROJECT_API UChasePlayer_BTTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UChasePlayer_BTTask(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true));
	//bool search_random = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true));
	//float search_radius = 150.0f;
	
};
