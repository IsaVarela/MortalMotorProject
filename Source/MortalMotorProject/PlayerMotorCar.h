// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "PlayerMotorCar.generated.h"

// Delegate signature
DECLARE_DELEGATE_OneParam(FOnGoldCollected, float);
DECLARE_MULTICAST_DELEGATE(FOnLevelUp);

class UPlayerUI;

UCLASS()
class MORTALMOTORPROJECT_API APlayerMotorCar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	APlayerMotorCar();

	FOnGoldCollected OnGoldCollectedDelegate;
	FOnLevelUp OnLevelUpDelegate;

	void Health(float damage);
	void Heal(float amount);
	void Accelerate();

	class USpringArmComponent* SpringArm;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
 
	class UChaosVehicleMovementComponent* VehicleMovementComponent;

	const float MAX_HEALTH = 100.f;
	float PlayerHealth;

	void HandleGoldCollected();
	void Break();
	void Steer(float x);	 
	void CameraRotation();

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* KillZoneCollisionSphere;

	UPROPERTY(VisibleAnywhere)
	int GoldAmount;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* ExpCurveFloat;

	UPROPERTY(VisibleAnywhere)
	int Level;

	//SFX
	UPROPERTY(EditDefaultsOnly)
	USoundBase* GoldCollectSoundCue;
 
	UPlayerUI* PlayerUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerUI> WidgetObject;

	//For killzone cleanup
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		


	
};
