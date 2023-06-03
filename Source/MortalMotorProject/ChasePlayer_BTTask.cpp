// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer_BTTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PlayerMotorCar.h"
#include "blackboar_keys.h"
 

UChasePlayer_BTTask::UChasePlayer_BTTask(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");

}

EBTNodeResult::Type UChasePlayer_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character and Enemy controller
	APlayerMotorCar* const Player = Cast<APlayerMotorCar> (UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	auto const controller = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());

	// get player location
	FVector const player_location = Player->GetActorLocation();

	controller->get_blackboard()->SetValueAsVector(bb_keys::target_location, player_location);


	//finish
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Type();


}
