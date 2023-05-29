// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IDamageable.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMine::AMine()
{
	PrimaryActorTick.bCanEverTick = false;

	MineBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineBody"));
	RootComponent = MineBody;

	ExplostionVFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion_VFX"));
	ExplostionVFX->bAutoActivate = false;
	ExplostionVFX->SetupAttachment(MineBody);

	TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);;
}

void AMine::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(DelayedHandle,this,&AMine::Explode, m_delayedExplosion,false);
}

void AMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMine::Explode()
{
	//set self destroy timer
	GetWorld()->GetTimerManager().SetTimer(SelfDestructDelay, this, &AMine::SelfDestroy, m_lifeTime, false);

	ExplostionVFX->SetWorldLocation(GetActorLocation());

	//vfx activate
	ExplostionVFX->Activate();

	//Scan for enemies to apply damage
	bool hit = UKismetSystemLibrary::SphereTraceMulti(
		this,
		GetActorLocation(),
		GetActorLocation() + FVector(0, 0, 0.001f),
		m_explosionRadius,
		TraceChannel,
		false,
		m_gnoredActors,
		EDrawDebugTrace::ForDuration,
		m_hits,
		false
	);

	//apply damage to enemies if were found and hit
	if (hit)
	{
		for (auto ele : m_hits)
		{
			AActor* HitActor = ele.GetActor();

			if (HitActor)
			{
				IIDamageable* Enemy = Cast<IIDamageable>(HitActor);
				if (Enemy)
				{
					Enemy->TakeDamge(m_damage);
				}
			}
		}
	}
}

void AMine::SelfDestroy()
{
	ExplostionVFX->Deactivate();
	ExplostionVFX->DestroyComponent();
	Destroy();
}


