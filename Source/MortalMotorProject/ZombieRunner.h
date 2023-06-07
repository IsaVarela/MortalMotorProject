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

	virtual void DestroyEnemy() override;

	virtual bool IsAlive() const override { return HealthPoints > 0; }

	///** called when something enters the trigger collision component */
	//UFUNCTION()
	//void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	///** called when something leaves the trigger collision component */
	//UFUNCTION()
	//void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// this os called when other colliders hit but not overlap
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

	// function to make the zombie follow the player's position
	void ChasePlayer(const FVector& TargetLocation) const;

	void BecomeRagdoll();
	void PlaySoundCueHitGround();

	//function to run when the hit points reach 0
	void Death();

	//function to restart particle system to make it look better
	void ParticleSystem();
	void DisableCollision();

	UPROPERTY()
		AAIController* ZombieController;

	UPROPERTY()
		APawn* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		UParticleSystemComponent* HitParticlesComponent;

	UPROPERTY()
		UParticleSystem* ParticleSystemTemplate;

	UPROPERTY()
		USoundCue* SoundCueBodyFall;

	UPROPERTY()
		USoundCue* SoundCueHitCar;

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

	UPROPERTY(EditAnywhere)
		bool bIsCollidingWithPlayer;

	UPROPERTY(EditAnywhere)
		bool bIsPsPlaying;

};