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
	TMap<USceneComponent*, bool> MSpawnPointsMap;
	float Timer = 0.f;

	void SpawnEnemy();
	void BruteForceSpawnEnemies();
	void SpawnFromPool();


	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> EnemyPrefabs;

	TQueue<AActor*> Pool;

	const int c_MaxSpawnActorsInScene = 15; //should always be same or less then MAX_ACTORS_IN_POOL
	const int c_MaxActorsInPool = 15;

	int m_spawnedActorsInScene = 0;
	int m_InitialspawnedActors = 0;
	bool bMaxSpawnReached;


public:	
	virtual void Tick(float DeltaTime) override;

	void PutEnemyBackInThePool(AActor* enemy);

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* NorthSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SouthSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* WestSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* EastSphere;

	//EAST
	UFUNCTION()
	void OnEastSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEastEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//WEST
	UFUNCTION()
	void OnWestSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWestEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	//NORTH
	UFUNCTION()
	void OnNorthSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNorthEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//SOUTH
	UFUNCTION()
	void OnSouthSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSouthEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
