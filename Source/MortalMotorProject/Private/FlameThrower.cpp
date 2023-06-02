// Fill out your copyright notice in the Description page of Project Settings.


#include "FlameThrower.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "MortalMotorProject/IDamageable.h"


// Sets default values
AFlameThrower::AFlameThrower():
	m_searchTimer(0.f), 
	m_attackTimer(0.f),
	InRange(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlameThrowerBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	FlameThrowerBody->SetupAttachment(RootComponent);

	//NiagaraFx 
	FireFxComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireFX"));
	FireFxComponent->bAutoActivate = false;
	FireFxComponent->SetupAttachment(FlameThrowerBody);

	//variables 
	//InRange = true; 
	TraceParams = FCollisionQueryParams();
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);
	SweepSphere = FCollisionShape::MakeSphere(Radius);
	m_offset = FVector(5000.f, 0, 0);
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
		//UE_LOG(LogTemp, Warning, TEXT("ScanTarget"));

	}

	if (Target != nullptr)
	{
		if (m_searchTimer >= SEARCH_TIME)
		{
			m_searchTimer = 0.f;
			CheckInRange();
			//UE_LOG(LogTemp, Warning, TEXT("CheckInRange"));

		}

		//FVector TargetLocation = Target->GetActorLocation(); 
		RotateTurret(DeltaTime);
		//UE_LOG(LogTemp, Warning, TEXT("RotateTurret"));

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
		InRange = false;
		StopFire();
		/*if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Target out of Range"));
		}*/
		ResetRotation();
	}
	else
		InRange = true;
}

void AFlameThrower::ScanTarget()
{
	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		FlameThrowerBody->GetComponentLocation(),
		FlameThrowerBody->GetComponentLocation() + m_offset,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		SweepSphere,
		TraceParams
	);

	if (bHit)
	{
		Target = HitResult.GetActor();

		if (Target && Target->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
		{
			DrawDebugSphere
			(
				GetWorld(),
				HitResult.ImpactPoint,
				50.f,
				12.f,
				FColor::Green,
				false,
				-1.f,
				20.f
			);
		}
		else
			Target = nullptr;
	}
}

void AFlameThrower::Fire()
{
	if (Target == nullptr)
		return;

	if (IIDamageable* Damage = Cast<IIDamageable>(Target))
	{
		FireFxComponent->Activate();

		UE_LOG(LogTemp, Warning, TEXT("ActivateNiagara"));

		if (Damage->IsAlive())
		{
			Damage->TakeDamge(Damge);
		}
		else
		{
			Target = nullptr;
			StopFire();
			ResetRotation();
		}
	}
}

void AFlameThrower::StopFire()
{
	FireFxComponent->Deactivate();
}

void AFlameThrower::ResetRotation()
{
	FlameThrowerBody->SetWorldRotation(InitialRotation);
}

