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

    virtual void PostInitializeComponents() override;

    virtual void TakeDamge(float damage) override;

     

    // Function to move the Zombie to a given location
    UFUNCTION()
        void MoveToLocation(const FVector& TargetLocation);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
        UAnimInstance* ZombieAnimInstance;
		

private:
    UPROPERTY(EditAnywhere, Category = "Movement")
		float MoveSpeed;

    UPROPERTY()
        TArray<UAnimMontage*> Hit_Montages;

    UPROPERTY()
        TArray<UAnimMontage*> Death_Montages;

    UPROPERTY()
		UAnimMontage* Hit_Montage01;

    UPROPERTY()
		UAnimMontage* Hit_Montage02;

    UPROPERTY()
        UAnimMontage* Death_Montage01;

    UPROPERTY()
        UAnimMontage* Death_Montage02;

	// Reference to the AIController
    UPROPERTY()
		class AAIController* ZombieAIController;
   
    UPROPERTY()
		APawn* PlayerPawn;

};

