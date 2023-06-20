// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "IDamageable.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h" 
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

	virtual void PostInitializeComponents() override;

	//overrides for the IDamageable interface 
	virtual void TakeDamge(float damage) override;

	virtual bool IsAlive() const override { return HealthPoints > 0; }

	/** called when something enters the trigger collision component */
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// function to make the zombie follow the player's position
	virtual void ChasePlayer(const FVector& TargetLocation) const;

	void PlaySoundCueHitGround();

	//function to run when the hit points reach 0
	void Death();

	//function to restart particle system to make it look better
	void ParticleSystem();

	void DisableCollision();

	virtual void ResetEnemy() override;

	UPROPERTY()
		AAIController* ZombieController;

	UPROPERTY()
		APawn* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		UParticleSystemComponent* HitParticlesComponent;

	UPROPERTY()
		UParticleSystem* Template_Blood;

	UPROPERTY()
		UParticleSystem* Template_Fire;

	UPROPERTY()
		USoundCue* SoundCueBodyFall;

	UPROPERTY()
		USoundCue* SoundCueHitCar;

	UPROPERTY()
		USoundCue* SoundCueApproach;

	UPROPERTY()
		UAudioComponent* AudioComponent;

	UPROPERTY()
		float AttackPower;

	UPROPERTY()
		bool bIsBurned;

	//These variables were made public and editable to allow more flexibility when changing meshes for the same zombie type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Hit_Montage01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Hit_Montage02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death_Montage01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* Death_Montage02;

private:

	void BecomeRagdoll();
	void KillEnemy(); //disables the enemy, hides it and puts it back in the pool

	UPROPERTY(EditDefaultsOnly)
		float HealthPoints = 100.f;

	UPROPERTY()
		TArray<UAnimMontage*> Hit_Montages;

	UPROPERTY()
		TArray<UAnimMontage*> Death_Montages;

	UPROPERTY(EditAnywhere)
		bool bIsCollidingWithPlayer;

	UPROPERTY(EditAnywhere)
		bool bIsPsPlaying;

	UPROPERTY(EditAnywhere)
	  mutable bool bIsSoundPlaying;

};