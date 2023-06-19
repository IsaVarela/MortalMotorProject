// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SphereComponent.h"
#include "Containers/Queue.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root SceneComp"));
	RootComponent = RootSceneComponent;

	//North
	NorthSphere = CreateDefaultSubobject<USphereComponent>(TEXT("North"));
	NorthSphere->SetSphereRadius(50.f);
	NorthSphere->SetupAttachment(RootComponent);
	NorthSphere->SetRelativeLocation(FVector(700.f, 0, 0));
	MSpawnPointsMap.Add(NorthSphere, true);

	//South
	SouthSphere = CreateDefaultSubobject<USphereComponent>(TEXT("South"));
	SouthSphere->SetSphereRadius(50.f);
	SouthSphere->SetupAttachment(RootComponent);
	SouthSphere->SetRelativeLocation(FVector(-700.f, 0, 0));
	MSpawnPointsMap.Add(SouthSphere, true);

	//West
	WestSphere = CreateDefaultSubobject<USphereComponent>(TEXT("West"));
	WestSphere->SetSphereRadius(50.f);
	WestSphere->SetupAttachment(RootComponent);
	WestSphere->SetRelativeLocation(FVector(0.f, -700.f, 0));
	MSpawnPointsMap.Add(WestSphere, true);

	//East
	EastSphere = CreateDefaultSubobject<USphereComponent>(TEXT("East"));
	EastSphere->SetSphereRadius(50.f);
	EastSphere->SetupAttachment(RootComponent);
	EastSphere->SetRelativeLocation(FVector(0, 700.f, 0));
	MSpawnPointsMap.Add(EastSphere, true);

	
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{ 
	Super::BeginPlay();

	FScriptDelegate ScriptDelegate;

	//EAST
	ScriptDelegate.BindUFunction(this, FName("OnEastSphereBeginOverlap"));
	EastSphere->OnComponentBeginOverlap.Add(ScriptDelegate);
	ScriptDelegate.Unbind();
	
	ScriptDelegate.BindUFunction(this, FName("OnEastEndOverlap"));
	EastSphere->OnComponentEndOverlap.Add(ScriptDelegate);

	//WEST
	ScriptDelegate.BindUFunction(this, FName("OnWestSphereBeginOverlap"));
	WestSphere->OnComponentBeginOverlap.Add(ScriptDelegate);
	ScriptDelegate.Unbind();

	ScriptDelegate.BindUFunction(this, FName("OnWestEndOverlap"));
	WestSphere->OnComponentEndOverlap.Add(ScriptDelegate);

	//North
	ScriptDelegate.BindUFunction(this, FName("OnNorthSphereBeginOverlap"));
	NorthSphere->OnComponentBeginOverlap.Add(ScriptDelegate);
	ScriptDelegate.Unbind();

	ScriptDelegate.BindUFunction(this, FName("OnNorthEndOverlap"));
	NorthSphere->OnComponentEndOverlap.Add(ScriptDelegate);

	//South
	ScriptDelegate.BindUFunction(this, FName("OnSouthSphereBeginOverlap"));
	SouthSphere->OnComponentBeginOverlap.Add(ScriptDelegate);
	ScriptDelegate.Unbind();

	ScriptDelegate.BindUFunction(this, FName("OnSouthEndOverlap"));
	SouthSphere->OnComponentEndOverlap.Add(ScriptDelegate);
}

void AEnemySpawner::SpawnEnemy()
{
	//if the pool is not full yet, create new objects
	if (m_poolSize < c_MaxActorsInPool)
	{
		BruteForceSpawnEnemies();
		UE_LOG(LogTemp, Warning, TEXT("BruteSpawn"));
	}

	//if the Pool is full, dequeue from pool
	else
	{
		SpawnFromPool();
		UE_LOG(LogTemp, Warning, TEXT("Pool"));
	}
}

void AEnemySpawner::BruteForceSpawnEnemies()
{
	for (const auto& element : MSpawnPointsMap)
	{
		//if spawn available
		if (element.Value == true)
		{
			if(EnemyPrefabs.Num()> 0)
			{
				const int32 RandomIndex = FMath::RandRange(0, EnemyPrefabs.Num() - 1);
				AActor* createdActor = GetWorld()->SpawnActor<AActor>(EnemyPrefabs[RandomIndex], element.Key->GetComponentLocation(), FRotator::ZeroRotator);
				Pool.Enqueue(createdActor);
				m_poolSize++;
				m_spawnedActorsInScene++;
			}
			
		}
	}
}

void AEnemySpawner::SpawnFromPool()
{
	for (const auto& element : MSpawnPointsMap)
	{
		//if spawn available
		if (element.Value == true)
		{
			if (EnemyPrefabs.Num() > 0)
			{
				m_spawnedActorsInScene++;
				AActor* returned = nullptr;
				Pool.Dequeue(returned);
				returned->SetActorLocation(element.Key->GetComponentLocation());
				Pool.Enqueue(returned);
			}

		}
	}
}

bool AEnemySpawner::IsSceneFull() const
{
	return m_spawnedActorsInScene >= c_MaxSpawnActorsInScene;
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (Timer >= 5.f)
	{
		SpawnEnemy();
		Timer = 0.f;
	}

}

void AEnemySpawner::OnEastSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MSpawnPointsMap[EastSphere] = false;
}

void AEnemySpawner::OnEastEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MSpawnPointsMap[EastSphere] = true;
}

void AEnemySpawner::OnWestSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MSpawnPointsMap[WestSphere] = false;
}

void AEnemySpawner::OnWestEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MSpawnPointsMap[WestSphere] = true;
}

void AEnemySpawner::OnNorthSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MSpawnPointsMap[NorthSphere] = false;
}

void AEnemySpawner::OnNorthEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MSpawnPointsMap[NorthSphere] = true;
}

void AEnemySpawner::OnSouthSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MSpawnPointsMap[SouthSphere] = false;
}

void AEnemySpawner::OnSouthEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MSpawnPointsMap[SouthSphere] = true;
}

