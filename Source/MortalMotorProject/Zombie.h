// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Zombie.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MORTALMOTORPROJECT_API AZombie : public AEnemy
{
	GENERATED_BODY()

public:
    AZombie();
    UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Skeleton;

    UPROPERTY(EditDefaultsOnly)
        UCapsuleComponent* CapsuleComponent;

    virtual void Tick(float DeltaSeconds) override;

    // Function to move the Zombie to a given location
    UFUNCTION()
        void MoveToLocation(const FVector& TargetLocation);

 

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
        float MoveSpeed;

    UPROPERTY()
    // Reference to the AIController
    class AAIController* ZombieAIController;
   
    UPROPERTY()
    APawn* PlayerPawn;

};

