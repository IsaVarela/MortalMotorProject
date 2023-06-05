// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class USphereComponent;

UCLASS()
class MORTALMOTORPROJECT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* NorthSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SouthSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* WestSphere;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* EastSphere;

	UFUNCTION()
	void OnEastSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
