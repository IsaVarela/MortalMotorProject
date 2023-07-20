// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "FlameThrower.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API AFlameThrower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlameThrower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ShowFlameThrower();

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UStaticMeshComponent* FlameThrowerBody;

private:

	void RotateTurret(float DeltaTime);

	void CheckInRange();

	void ScanTarget();

	void Fire();

	void StopFire();

	void ResetRotation();

	bool InRange;
	AActor* Target;

	UPROPERTY(EditDefaultsOnly)
		float Radius = 2000.f;

	UPROPERTY(EditDefaultsOnly)
		float TurretRotationSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly)
		float Damge = 20.f;

	UPROPERTY(EditDefaultsOnly)
		float FireRate = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "NiagraFx")
		UNiagaraComponent* FireFxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		UAudioComponent* AudioComp; 

	float m_searchTimer;
	float m_attackTimer;
	float const SEARCH_TIME = 2.f;


	FVector m_offset;
	FCollisionShape SweepSphere;
	FCollisionQueryParams TraceParams;
	FHitResult HitResult;
	FRotator InitialRotation;

};
