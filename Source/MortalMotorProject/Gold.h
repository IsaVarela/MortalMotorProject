// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Gold.generated.h"

class UCurveFloat;

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

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category = "Timeline")
	UCurveFloat* CurveFloat;

	FTimeline CurveFTimeline;

	FVector StartLoc;
	FVector EndLoc;
	AActor* Player;

private:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GoldMesh;

	UPROPERTY(EditDefaultsOnly)
	float AttractionSpeedMult = 0.7f;

	UFUNCTION()
	void TimelineProgress(float val);

	UFUNCTION()
	void TimeLineFinish();

	void OnGoldCollected();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable)
	void AttractTowardsPlayer(AActor* playerActor);

	

};
