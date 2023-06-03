// Fill out your copyright notice in the Description page of Project Settings.


#include "LandMineSpawner.h"
#include "Mine.h"

// Sets default values
ALandMineSpawner::ALandMineSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALandMineSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALandMineSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bHasEnabled) { return; }

	if (m_spawnTimer >= m_spawnDelay)
	{
		GetWorld()->SpawnActor<AMine>(MinePrefab, GetActorLocation(), FRotator::ZeroRotator);
		m_spawnTimer = 0.f;
	}

	m_spawnTimer += DeltaTime;

}

void ALandMineSpawner::EnableLandMineSpawner()
{
	bHasEnabled = true;
}

