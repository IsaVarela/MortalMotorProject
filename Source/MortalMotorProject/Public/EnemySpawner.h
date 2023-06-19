// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <queue>
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
	bool IsSceneFull() const;

	// original declaration, changed to an array to test the spawn of enemies of the same type but with different meshes.
	/*UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EnemyPrefab;*/

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> EnemyPrefabs;

	//Pool of spawned Actors
	//TQueue<AActor*> Pool;
	TQueue<AActor*> Pool;

	const int c_MaxSpawnActorsInScene = 5;
	const int c_MaxActorsInPool = 5;
	int m_spawnedActorsInScene = 0;
	int m_poolSize = 0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
