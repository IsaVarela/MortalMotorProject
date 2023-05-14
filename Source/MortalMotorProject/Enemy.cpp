// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);
	RootComponent = Body;

	HitParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffectComponent"));
	HitParticlesComponent->SetupAttachment(RootComponent);
	HitParticlesComponent->bAutoActivate = false;
	
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::TakeDamge(float damage)
{
	HealthPoints = FMath::Max(0, HealthPoints - damage);

	if (HealthPoints == 0.f)
	{
		Destroy();
	}
	else 
	{
		if (HitParticlesComponent)
		{
			HitParticlesComponent->Activate();
		}
	}
}


