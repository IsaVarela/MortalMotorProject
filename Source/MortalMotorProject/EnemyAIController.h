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
	AEnemyAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* const pawn) override;
	UBlackboardComponent* get_blackboard() const;

	// executes logic
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class UBehaviorTreeComponent* BehaviorTreeComponent;
	// defines logic
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardComponent* BlackboardComponent;

private:

	
	
};
