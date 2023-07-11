// Fill out your copyright notice in the Description page of Project Settings.


#include "DeliverTarget.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerNavigationSystem.h"

// Sets default values
ADeliverTarget::ADeliverTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Deliver Target"));
	RootComponent = DetectSphere;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFX"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ADeliverTarget::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, FName("OnBeginOverlap"));
	DetectSphere->OnComponentBeginOverlap.Add(ScriptDelegate);

	DeactivateTarget();
}

// Called every frame
void ADeliverTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADeliverTarget::ActivateTarget()
{
	ParticleSystemComponent->ActivateSystem();
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ADeliverTarget::DeactivateTarget()
{
	ParticleSystemComponent->DeactivateSystem();
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADeliverTarget::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UPlayerNavigationSystem::ReachTarget(this);
	}
}

