// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliverTarget.h"
#include "Components/SphereComponent.h"

// Sets default values
ADeliverTarget::ADeliverTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Deliver Target"));
	RootComponent = DetectSphere;
	//DeactivateTarget();
}

// Called when the game starts or when spawned
void ADeliverTarget::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADeliverTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeliverTarget::ActivateTarget()
{
	this->SetActorHiddenInGame(false);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADeliverTarget::DeactivateTarget()
{
	this->SetActorHiddenInGame(true);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

