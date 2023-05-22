// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
 
 

AZombie::AZombie() 
{
	PrimaryActorTick.bCanEverTick = true;

	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	Skeleton->SetupAttachment(RootComponent);

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollisionComponent"));

	// Create the character movement component
	UCharacterMovementComponent* MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

    // Get the AIController of the Zombie
	ZombieAIController = Cast<AAIController>(GetController());

  

 
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
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("WE HAVE A VALID CONTROLLER"));
        // Check if the player's pawn reference is valid
        if (!PlayerPawn)
        {
            // Get the player's pawn
            PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        }

        if (PlayerPawn)
        {
           // GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("WE HAVE A PLAYER REFERENCE"));
            // Move the Zombie towards the player
            MoveToLocation(PlayerPawn->GetActorLocation());
        }
    }
}

void AZombie::MoveToLocation(const FVector& TargetLocation)
{
    UNavigationSystemV1* NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    

    if (NavigationSystem)
    {
        
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("WE HAVE A VALID NAVMESH"));
        
    }



    if (ZombieAIController)
    {
        FString MoveStatusString = FString::Printf(TEXT("Move Status: %d"), ZombieAIController->GetMoveStatus());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, MoveStatusString);
         
         float Distance = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Vector Value: %f"), Distance));
         ZombieAIController->ResumeMove(true);
        // Call MoveToLocation on the AIController
        ZombieAIController->MoveToLocation(TargetLocation);
    }
}