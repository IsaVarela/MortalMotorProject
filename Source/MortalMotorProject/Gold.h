// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gold.generated.h"

// Delegate signature
DECLARE_DELEGATE(FOnGoldCollectedSignature)

UCLASS()
class MORTALMOTORPROJECT_API AGold : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGold();

	static FOnGoldCollectedSignature s_OnGoldCollected;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GoldMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnGoldCollected();

	UFUNCTION(BlueprintCallable)
	void AttractTowardsPlayer(const AActor* playerActor);


	
};
