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
 
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void ChasePlayer(const FVector& TargetLocation) const override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
 
	void Attack() const;
  
	/*UFUNCTION()
		void OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/
  
	UPROPERTY()
		mutable bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TArray<UAnimMontage*> AttackAnims;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* AttackArea;
  
};