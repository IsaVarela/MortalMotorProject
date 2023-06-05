// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SphereComponent.h"

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

	//South
	SouthSphere = CreateDefaultSubobject<USphereComponent>(TEXT("South"));
	SouthSphere->SetSphereRadius(50.f);
	SouthSphere->SetupAttachment(RootComponent);
	SouthSphere->SetRelativeLocation(FVector(-700.f, 0, 0));

	//West
	WestSphere = CreateDefaultSubobject<USphereComponent>(TEXT("West"));
	WestSphere->SetSphereRadius(50.f);
	WestSphere->SetupAttachment(RootComponent);
	WestSphere->SetRelativeLocation(FVector(0.f, -700.f, 0));

	//East
	EastSphere = CreateDefaultSubobject<USphereComponent>(TEXT("East"));
	EastSphere->SetSphereRadius(50.f);
	EastSphere->SetupAttachment(RootComponent);
	EastSphere->SetRelativeLocation(FVector(0, 700.f, 0));

	
	
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	FScriptDelegate ds;
	ds.BindUFunction(this, FName("OnEastSphereBeginOverlap"));
	EastSphere->OnComponentBeginOverlap.Add(ds);
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::OnEastSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("HIT"));
}

