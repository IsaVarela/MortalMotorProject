// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TimelineComponent.h"
#include "Gold.h"

FOnGoldCollectedSignature AGold::s_OnGoldCollected;

// Sets default values
AGold::AGold()
{
	PrimaryActorTick.bCanEverTick = true;

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gold Mesh"));
}

// Called when the game starts or when spawned
void AGold::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGold::OnGoldCollected()
{
	s_OnGoldCollected.ExecuteIfBound();
}

void AGold::AttractTowardsPlayer(const AActor* playerActor)
{
	GoldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

   
}

