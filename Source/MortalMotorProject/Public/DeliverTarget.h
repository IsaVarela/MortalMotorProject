// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeliverTarget.generated.h"

UCLASS()
class MORTALMOTORPROJECT_API ADeliverTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeliverTarget();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateTarget();
	void DeactivateTarget();

private:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* DetectSphere;

};
