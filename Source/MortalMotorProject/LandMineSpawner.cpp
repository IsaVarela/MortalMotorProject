// Fill out your copyright notice in the Description page of Project Settings.


#include "LandMineSpawner.h"

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

}

void ALandMineSpawner::EnableLandMineSpawner()
{
	UE_LOG(LogTemp, Warning, TEXT("Enabled"));
	bHasEnabled = true;
}

