// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "IDamageable.h"
#include "ZombieRunner.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API AZombieRunner : public ACharacter, public IIDamageable 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieRunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		UAnimInstance* ZombieAnimInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	//overrides for the IDamageable interface 
	virtual void TakeDamge(float damage) override;

	virtual void DestroyEnemy() override;

	virtual bool IsAlive() const override { return HealthPoints > 0; }

	// function to make the zombie follow the player's position
	void ChasePlayer(const FVector& TargetLocation) const;

	//function to run when the hit points reach 0
	void Death();

	UPROPERTY()
	AAIController* ZombieController;

	UPROPERTY()
		APawn* Player;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystemComponent* HitParticlesComponent;

private:

	UPROPERTY(EditDefaultsOnly)
		float HealthPoints = 100.f;

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
 
};
