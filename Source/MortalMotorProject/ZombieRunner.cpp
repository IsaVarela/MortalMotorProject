// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieRunner.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AZombieRunner::AZombieRunner()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZombieRunner::BeginPlay()
{
	Super::BeginPlay();

	ZombieController = Cast<AAIController>(GetController());

	Player = UGameplayStatics::GetPlayerPawn(this, 0);

	if(Player)
	{
		ChasePlayer(Player->GetActorLocation());
	}
	
}

// Called every frame
void AZombieRunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombieRunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AZombieRunner::ChasePlayer(const FVector& TargetLocation)
{
	if(ZombieController)
	{
		ZombieController->MoveToLocation(TargetLocation);
	}
}

