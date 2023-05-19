// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

AZombie::AZombie() 
{
	PrimaryActorTick.bCanEverTick = true;

	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	Skeleton->SetupAttachment(RootComponent);

	// Create the character movement component
	UCharacterMovementComponent* MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
 
}

void AZombie::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!ZombieAIController)
    {
        // Get the AIController of the Zombie
        ZombieAIController = Cast<AAIController>(GetController());
    }

    if (ZombieAIController)
    {
        // Check if the player's pawn reference is valid
        if (!PlayerPawn)
        {
            // Get the player's pawn
            PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        }

        if (PlayerPawn)
        {
            // Move the Zombie towards the player
            MoveToLocation(PlayerPawn->GetActorLocation());
        }
    }
}

void AZombie::MoveToLocation(const FVector& TargetLocation)
{
    // Get the AIController of the Zombie
    //AAIController* ZombieAIController = Cast<AAIController>(GetController());
    if (ZombieAIController)
    {
        // Call MoveToLocation on the AIController
        ZombieAIController->MoveToLocation(TargetLocation);
    }
}