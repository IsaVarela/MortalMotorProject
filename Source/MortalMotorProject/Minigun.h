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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AActor* Target;

public:	

	virtual void Tick(float DeltaTime) override;

private:
	void ScanForTarget();
	void ValidateTarget();
	void RotateTowardsTarget(float deltaTime);

private:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float Radius = 3000.f;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TurretRotationSpeed = 10.f;

	float m_searchTimer;
	float const SEARCH_TIME = 2.f;

	//Trace variables
	FCollisionQueryParams TraceParams;
	FCollisionShape SweepSphere;
	FVector offset;
	FHitResult Outhit;


};
