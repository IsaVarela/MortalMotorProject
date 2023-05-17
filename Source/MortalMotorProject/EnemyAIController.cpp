// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
 
 


AEnemyAIController::AEnemyAIController(FObjectInitializer const& objectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("B:/MortalMotorProject/Content/Blueprints/Enemies/AI/Enemy_BT.uasset"));
	if(obj.Succeeded())
	{
		BehaviorTree = obj.Object;
	}
	BehaviorTreeComponent = objectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComponent = objectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("blackboardComp"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	BehaviorTreeComponent->StartTree((*BehaviorTree));
	 
}

void AEnemyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if(BlackboardComponent)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* AEnemyAIController::get_blackboard() const
{
	return BlackboardComponent;
}

