// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PlayerMotorCar.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API APlayerMotorCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	APlayerMotorCar();

private:
	void HandleGoldCollected();

	UPROPERTY(VisibleAnywhere)
	int GoldAmount;
	
};
