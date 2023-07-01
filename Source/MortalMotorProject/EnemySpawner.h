// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class USphereComponent;

UCLASS()
class MORTALMOTORPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

private:
	
	float Timer = 0.f;

	void SpawnEnemy();
	void BruteForceSpawnEnemies();
	void SpawnFromPool();
	bool CheckOverlap(USceneComponent* SpawnPoint);
	USceneComponent* GetRandomSpawnPoint() const;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> EnemyPrefabs;

	TArray<USceneComponent*> SpawnPoints;
	TQueue<AActor*> Pool;

	FCollisionObjectQueryParams QueryParams;

	const int c_MaxSpawnActorsInScene = 15; //should always be same or less then MAX_ACTORS_IN_POOL
	const int c_MaxActorsInPool = 15;

	int m_spawnedActorsInScene = 0;
	int m_InitialspawnedActors = 0;
	bool bMaxSpawnReached;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* RootSceneComponent;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* NorthSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* EastSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* WestSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* SouthSpawnPoint;


public:	
	virtual void Tick(float DeltaTime) override;

	void PutEnemyBackInThePool(AActor* enemy);

};
