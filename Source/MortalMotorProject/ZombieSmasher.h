// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MortalMotorProject/ZombieRunner.h"
#include "ZombieSmasher.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API AZombieSmasher : public AZombieRunner
{
	GENERATED_BODY()

public:
	AZombieSmasher();

	void AttackPlayer(AActor* OtherActor, float RecoilForce, float AttackDamage, bool PlaySound);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* AttackingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* AttackingMiss;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* AttackingDefault;

	 

	UPROPERTY()
	mutable bool bInAttackCollider;

	UPROPERTY()
	mutable int AnimIndex;

	
 
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void ChasePlayer(const FVector& TargetLocation) const override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
 
	void PlayAttackAnim() const;

	UFUNCTION()
		void OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
						 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TArray<UAnimMontage*> AttackAnims;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* AttackArea;
  
};
