// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mine.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API AMine : public AActor
{
	GENERATED_BODY()
	
public:	

	AMine();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


private:
	float m_lifeTime = 3.0f;
	float m_delayedExplosion = 1.5f;
	float m_explosionRadius = 700.f;
	float m_damage = 1000.f;

	FTimerHandle DelayedHandle;
	FTimerHandle SelfDestructDelay;
	TArray<FHitResult> m_hits;
	TArray<AActor*> m_gnoredActors;
	ETraceTypeQuery TraceChannel;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* ExplostionVFX;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MineBody;

	void Explode();
	void SelfDestroy();
	
};
