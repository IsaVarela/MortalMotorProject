// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Minigun.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API AMinigun : public AActor
{
	GENERATED_BODY()
	
public:	

	AMinigun();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	UStaticMeshComponent* TurretBody;


public:	

	virtual void Tick(float DeltaTime) override;

private:
	void ScanForTarget();
	void ValidateTarget();
	void RotateTowardsTarget(float deltaTime);
	void Shoot();
	void StopShoot();

private:

	AActor* Target;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting-Params")
	float Radius = 3000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting-Params")
	float TurretRotationSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting-Params")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting-Params")
	float FireRate = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystemComponent* ShootVfxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystemComponent* SmokeVfxComponent;

	float m_searchTimer;
	float m_attackTimer;
	float const SEARCH_TIME = 2.f;

	//Trace variables
	FCollisionQueryParams TraceParams;
	FCollisionShape SweepSphere;
	FVector m_offset;
	FHitResult Outhit;


};
