// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandMineSpawner.generated.h"

class AMine;

UCLASS()
class MORTALMOTORPROJECT_API ALandMineSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandMineSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void EnableLandMineSpawner();

private:
	bool bHasEnabled;
	float m_spawnTimer = 0.f;
	float m_spawnDelay = 3.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMine> MinePrefab;
};
