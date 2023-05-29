// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDamageable.h"
#include "GameFramework/Pawn.h"
#include "Components/ArrowComponent.h"
#include "Enemy.generated.h"


UCLASS()
class MORTALMOTORPROJECT_API AEnemy : public APawn, public IIDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamge(float damage) override;

	virtual void DestroyEnemy() override;

	virtual bool IsAlive() const override { return HealthPoints > 0; }

	

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* HitParticlesComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Body;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* Arrow;

private:

	UPROPERTY(EditDefaultsOnly)
	float HealthPoints = 100.f;

	

};
