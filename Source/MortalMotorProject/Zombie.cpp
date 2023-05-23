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
#include "Animation/AnimInstance.h"
 
 

AZombie::AZombie() 
{
	PrimaryActorTick.bCanEverTick = true;

    // attach the skeletal Mesh component necessary to add the zombie mesh and anim blueprints 
	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	Skeleton->SetupAttachment(RootComponent);

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollisionComponent"));

	// Create the character movement component
	UCharacterMovementComponent* MovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

    // Get the AIController of the Zombie
	ZombieAIController = Cast<AAIController>(GetController());

    //Get Anim montages
    Hit_Montage01 = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Juan_Active_Branch/Enemies/Zombie_03/Anim/Zombie_Reaction_Hit_01_Montage_Retargeted.Zombie_Reaction_Hit_01_Montage_Retargeted'"));
    Hit_Montage02 = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Juan_Active_Branch/Enemies/Zombie_03/Anim/Zombie_Reaction_Hit_02_Montage_Retargeted.Zombie_Reaction_Hit_02_Montage_Retargeted'"));
    Death_Montage01 = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Juan_Active_Branch/Enemies/Zombie_03/Anim/ZombieDeath_01_Montage_Retargeted.ZombieDeath_01_Montage_Retargeted'"));
    Death_Montage02 = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/Juan_Active_Branch/Enemies/Zombie_03/Anim/ZombieDeath_02_Montage_Retargeted.ZombieDeath_02_Montage_Retargeted'"));
	
}

void AZombie::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    // after the constructor is completed check if the components were added and start assigning
    if(Skeleton && Skeleton->GetAnimInstance())
    {
        ZombieAnimInstance = Skeleton->GetAnimInstance();
    }

    if (Hit_Montage01)
    {
        Hit_Montages.Add(Hit_Montage01);
    }

    if (Hit_Montage02)
    {
        Hit_Montages.Add(Hit_Montage02);
    }

    if (Death_Montage01)
    {
        Death_Montages.Add(Death_Montage01);
    }

    if (Death_Montage02)
    {
        Death_Montages.Add(Death_Montage02);
    }

    

}

void AZombie::TakeDamge(float damage)
{
    // run the original inherited code from Enemy class
    Super::TakeDamge(damage);

    // playing a random index from an array that contains the animations when the zombie is hit by something
    if (Hit_Montages.Num() > 0)
    {
	    int32 RandomIndex = FMath::RandRange(0, Hit_Montages.Num() - 1);
        ZombieAnimInstance->Montage_Play(Hit_Montages[RandomIndex], 1.0f);
      //  GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Montage array size: %i"), Hit_Montages.Num()));//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("RandomIndex: %i"), RandomIndex));
    }

   if (!IsAlive())
   {
	   if(Death_Montages.Num() > 0)
	   {
           int32 RandomIndex = FMath::RandRange(0, Death_Montages.Num() - 1);
           ZombieAnimInstance->Montage_Play(Death_Montages[RandomIndex], 1.0f);
	   }
   }
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
        
       // GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("WE HAVE A VALID NAVMESH"));
        
    }



    if (ZombieAIController)
    {
        FString MoveStatusString = FString::Printf(TEXT("Move Status: %d"), ZombieAIController->GetMoveStatus());
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, MoveStatusString);
         
         float Distance = FVector::Dist(this->GetActorLocation(), PlayerPawn->GetActorLocation());
        // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Vector Value: %f"), Distance));
         ZombieAIController->ResumeMove(true);
        // Call MoveToLocation on the AIController
        ZombieAIController->MoveToLocation(TargetLocation);
    }
}