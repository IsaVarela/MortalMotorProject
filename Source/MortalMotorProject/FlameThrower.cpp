// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameThrower.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "MortalMotorProject/IDamageable.h" 
#include "MortalMotorProject/ZombieRunner.h"


// Sets default values
AFlameThrower::AFlameThrower():
	m_searchTimer(0.f), 
	m_attackTimer(0.f),
	InRange(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlameThrowerBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	FlameThrowerBody->SetupAttachment(RootComponent);

	//NiagaraFx 
	FireFxComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireFX"));
	FireFxComponent->bAutoActivate = false;
	FireFxComponent->SetupAttachment(FlameThrowerBody);

	// SFX 
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("FlameThrowerSFX")); 
	AudioComp->bAutoActivate = false; 

	//variables 
	//InRange = true; 
	TraceParams = FCollisionQueryParams();
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);
	SweepSphere = FCollisionShape::MakeSphere(Radius);
	m_offset = FVector(0, 0, 0.1f);
	InitialRotation = FlameThrowerBody->GetComponentRotation();
}

// Called when the game starts or when spawned
void AFlameThrower::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlameThrower::Tick(float DeltaTime)
{
	if (!InRange)
		return;
	Super::Tick(DeltaTime);

	if (Target == nullptr)
	{
		ScanTarget();
	}

	if (Target != nullptr)
	{
		if (m_searchTimer >= SEARCH_TIME)
		{
			m_searchTimer = 0.f;
			CheckInRange();
		}

		RotateTurret(DeltaTime);

		if (m_attackTimer >= FireRate && Target)
		{
			Fire();
			m_attackTimer = 0.f;
		}

		m_attackTimer += DeltaTime;
		m_searchTimer += DeltaTime;
	}
}

void AFlameThrower::ShowFlameThrower()
{
	InRange = true;
	FlameThrowerBody->SetVisibility(true);
}

void AFlameThrower::RotateTurret(float DeltaTime)
{
	if (Target == nullptr)
		return;

	FVector Direction = (Target->GetActorLocation() - FlameThrowerBody->GetComponentLocation().GetSafeNormal());

	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	TargetRotation.Pitch = 0.f;
	TargetRotation.Roll = 0.f;

	FRotator NewRotation = FMath::Lerp(FlameThrowerBody->GetComponentRotation(), TargetRotation, TurretRotationSpeed * DeltaTime);

	FlameThrowerBody->SetWorldRotation(NewRotation);
}

void AFlameThrower::CheckInRange()
{
	if (Target == nullptr)
		return;

	float Distance = this->GetDistanceTo(Target);

	//UE_LOG(LogTemp, Warning, TEXT("Distance: %f, Radius %f"), Distance, Radius);

	if (Distance > Radius)
	{
		Target = nullptr;
		StopFire();
		ResetRotation();
	}
}

void AFlameThrower::ScanTarget()
{
	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		FlameThrowerBody->GetComponentLocation(),
		FlameThrowerBody->GetComponentLocation() + m_offset,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1,
		SweepSphere,
		TraceParams
	);

	if (bHit)
	{
		Target = HitResult.GetActor();
	}
}

void AFlameThrower::Fire()
{
	if (Target == nullptr)
		return;

	if (!AudioComp->IsPlaying())
	{
		AudioComp->Play(); 
	}

	IIDamageable* Damage = Cast<IIDamageable>(Target); 
	FireFxComponent->Activate();

	

	if (Damage && Damage->IsAlive())
	{
		Damage->TakeDamge(Damge);
		if(Target->IsA<AZombieRunner>())
		{
			AZombieRunner* Zombie = Cast<AZombieRunner>(Target);
			Zombie->bIsBurned = true;
		}		
	}
	if(!Damage->IsAlive())
	{
		Target = nullptr;
		StopFire();
		//ResetRotation();
	}
}

void AFlameThrower::StopFire()
{
	AudioComp->Stop(); 
	FireFxComponent->Deactivate();
}

void AFlameThrower::ResetRotation()
{
	FlameThrowerBody->SetWorldRotation(InitialRotation);
}

