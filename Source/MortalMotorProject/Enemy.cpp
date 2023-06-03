// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ArrowComponent.h"

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

	// this helps to set the proper orientation of the mesh in the editor
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	
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
	// rewrote this part to use the IsAlive function since it already returns the hitpoint count
	if(IsAlive())
	{
		if (HitParticlesComponent)
		{
			HitParticlesComponent->Activate();
		}
	}
	else
	{
		// added a delay to the destruction of the objects inheriting from this class to allow some room to play death animations or additional code 
		const float delay = 6.0f;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::DestroyEnemy, delay, false);
	}

	/*if (HealthPoints == 0.f)
	{
		Destroy();
	}
	else 
	{
		if (HitParticlesComponent)
		{
			HitParticlesComponent->Activate();
		}
	}*/
}

void AEnemy::DestroyEnemy()
{
	Destroy();
}



