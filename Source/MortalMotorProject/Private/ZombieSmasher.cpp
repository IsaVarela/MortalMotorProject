// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSmasher.h"

AZombieSmasher::AZombieSmasher()
{
}

void AZombieSmasher::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieSmasher::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    PositionToPlayer();
}

void AZombieSmasher::ChasePlayer(const FVector& TargetLocation) const
{
	 
	Super::ChasePlayer(TargetLocation);

}

void AZombieSmasher::PositionToPlayer()
{
	const FVector zombiePosition = GetActorLocation();
    const FVector playerPosition = Player->GetActorLocation();
    const FVector playerForwardDirection = Player->GetActorForwardVector();
    const FVector playerToZombie = zombiePosition - playerPosition;

    // Calculate the dot product between the player's forward direction and the vector from player to zombie
    const float dotProduct = playerForwardDirection.Dot(playerToZombie);

    if (dotProduct > 0.0f) {
        // Zombie is in front play block anim 
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie IS in front"));
    }
    else {
        // Zombie is approaching from the sides, run the attack code
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie is NOT front"));
    }
}
