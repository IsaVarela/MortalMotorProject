// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SphereComponent.h"
#include "Containers/Queue.h"
#include "IDamageable.h"
#include "CollisionQueryParams.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
	:
	QueryParams(FCollisionObjectQueryParams())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root SceneComp"));
	RootComponent = RootSceneComponent;

	//North
	NorthSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("North Spawn"));
	NorthSpawnPoint->SetupAttachment(RootComponent);

	//South
	SouthSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("South Spawn"));
	SouthSpawnPoint->SetupAttachment(RootComponent);

	//West
	WestSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("West Spawn"));
	WestSpawnPoint->SetupAttachment(RootComponent);

	//East
	EastSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("East Spawn"));
	EastSpawnPoint->SetupAttachment(RootComponent);

	QueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel3);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{ 
	Super::BeginPlay();

	//populate array of spawn points
	SpawnPoints.Add(NorthSpawnPoint);
	SpawnPoints.Add(EastSpawnPoint);
	SpawnPoints.Add(SouthSpawnPoint);
	SpawnPoints.Add(WestSpawnPoint);
}

void AEnemySpawner::SpawnEnemy()
{
	//if the pool is not full yet, create new objects
	if (!bMaxSpawnReached)
	{
		BruteForceSpawnEnemies();
	}

	//if the Pool is full, dequeue from pool
	else if(!Pool.IsEmpty())
	{
		SpawnFromPool();
	}
}

void AEnemySpawner::BruteForceSpawnEnemies()
{
	USceneComponent* TempSpawnPoint = GetRandomSpawnPoint();
	if (!CheckOverlap(TempSpawnPoint))
	{
		if (EnemyPrefabs.Num() > 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, EnemyPrefabs.Num() - 1);
			AActor* createdActor = GetWorld()->SpawnActor<AActor>(EnemyPrefabs[RandomIndex], TempSpawnPoint->GetComponentLocation(), FRotator::ZeroRotator);

			m_spawnedActorsInScene++;
			m_InitialspawnedActors++;
			if (m_InitialspawnedActors == c_MaxActorsInPool)
			{
				bMaxSpawnReached = true;
			}
		}
	}
}

void AEnemySpawner::SpawnFromPool()
{
	USceneComponent* TempSpawnPoint = GetRandomSpawnPoint();
	if (!CheckOverlap(TempSpawnPoint))
	{
		if (EnemyPrefabs.Num() > 0)
		{
			if (Pool.IsEmpty()) { return; }

			auto temp = Pool.Peek();

			IIDamageable* Enemy = Cast<IIDamageable>(*temp);
			if (Enemy && !Enemy->IsAlive())
			{
				//UE_LOG(LogTemp, Warning, TEXT("spawn from pool, the enemy is dead %s"), *(*temp)->GetName());
				Enemy->ResetEnemy(TempSpawnPoint->GetComponentLocation());
				Pool.Dequeue(*temp);
				m_spawnedActorsInScene++;
				return;
			}
		}
	}

	
}

bool AEnemySpawner::CheckOverlap(USceneComponent* SpawnPoint)
{
	FVector Center = SpawnPoint->GetComponentLocation();
	float Radius = 200.f; 
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);

	bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps, Center, FQuat::Identity, QueryParams, CollisionShape);

	return bHasOverlap;
	
}

USceneComponent* AEnemySpawner::GetRandomSpawnPoint() const
{
	int index = FMath::RandRange(0, 3);

	return SpawnPoints[index];
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (Timer >= 1.f && m_spawnedActorsInScene < c_MaxSpawnActorsInScene)
	{
		SpawnEnemy();
		Timer = 0.f;
	}

}

void AEnemySpawner::PutEnemyBackInThePool(AActor* enemy)
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy died, adding him to the pool name: %s"), *enemy->GetName());
	Pool.Enqueue(enemy);
	m_spawnedActorsInScene--;
}
