// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	

private:

	// executes logic
	UPROPERTY(EditInstanceOnly)
		class UBehaviorTreeComponent* BehaviorTreeComponent;
	// defines logic
	UPROPERTY(EditInstanceOnly)
		class UBehaviorTree* BehaviorTree;

		class UBlackBoardComponent* BlackBoard;
	
};
