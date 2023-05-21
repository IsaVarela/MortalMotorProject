// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PlayerUI.h"
#include "PlayerMotorCar.generated.h"

// Delegate signature
DECLARE_DELEGATE_OneParam(FOnGoldCollected, float);
DECLARE_MULTICAST_DELEGATE(FOnLevelUp);

UCLASS()
class MORTALMOTORPROJECT_API APlayerMotorCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	APlayerMotorCar();

	FOnGoldCollected OnGoldCollectedDelegate;
	FOnLevelUp OnLevelUpDelegate;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:

	class UChaosVehicleMovementComponent* VehicleMovementComponent;
	class USpringArmComponent* SpringArm;

	void HandleGoldCollected();
	void Accelerate();
	void Break();
	void Steer(float x);
	void CameraRotation();

	UPROPERTY(VisibleAnywhere)
	int GoldAmount;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ExpCurveFloat;

	UPROPERTY(VisibleAnywhere)
	int Level;

	UPlayerUI* PlayerUI;

	UPROPERTY(BlueprintReadWrite,meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerUI> WidgetObject;


	
};
