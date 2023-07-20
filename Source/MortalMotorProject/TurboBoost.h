// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/ArrowComponent.h"
#include "TurboBoost.generated.h"


class APlayerMotorCar;


USTRUCT(BlueprintType)
struct FVehiclePropulsion
{
	GENERATED_BODY()

	// has propulsion been applied since the begining 
	bool HasStarted = false;

	// the applied piston engine throttle of the vehicle between -1 and 1 
	float PistonEngineThrottle = 0.f;

	// the applied jet engine throttle between -1 and 1 
	float JetEngineThrottle = 0.f;

	// the amount of jet engine power currently available 
	float CurrentJetEnginePower = 0.f;

	// the max amount of jet engine power 
	float MaxJetEnginePower = 2000.f;

	// boost to the thrust form a turbo type 
	float Boost = 0.f;

	// how much scale by when using a turbo boost 
	float BoostScale = 1.f;

	// the thrust strength of the current turbo between 0 and 1 
	float TurboThrottle = 0.f;

	// current gear position 
	float CurrentGearPosition = 0.f;

	// last gear position 
	float LastGearPosition = 0.f;

	// the amount of time spent in the current gear 
	float GearTime = 0.f;
};


USTRUCT(BlueprintType)
struct FVehicleControl
{
	GENERATED_BODY()

	// get the brake throttle position. the more brake we apply, the less throttle we allow 
	float GetBrakedThrottle() const
	{
		return ThrottleInput * (1.f - BrakePosition);
	}

	// the steering value between -1 and 1 
	float SteeringInput = 0.f;

	// the real position derived from the steering input 
	float SteeringPosition = 0.f;

	// the thrust force between -1 and 1 
	float RawThrottleInput = 0.f;

	// the thrust force between -1 and 1
	float ThrottleInput = 1.f;

	// the current braking force 
	float BrakeInput = 0.f;

	// brake is often just a digital switch between 0 and 1 
	float BrakePosition = 0.f;
};


USTRUCT(BlueprintType)
struct FVehiclePhysics
{
	GENERATED_BODY()

		FVehiclePhysics() = default;

	FVehiclePhysics(float amount, float duration, const FVector& direction) :
		Amount(amount),
		Timer(0.f),
		Duration(duration),
		Direction(direction)
	{}

	// boost to the thrust 
	float Amount = 2.f;

	// general timer 
	float Timer = 0.f;

	// duration to use for applying boost 
	float Duration = 5.f;

	// direction of the boost 
	FVector Direction = FVector::ZeroVector;
};


USTRUCT(BlueprintType)
struct FPhysicsVelocityData
{
	GENERATED_BODY()

	// set the car velocity and direction 
	void SetVelocities(const FVector& linearVelocity, const FVector& angularVelocity, const FVector& xDirection);

	// current speed of the vehicle 
	float Speed = 1100.f;

	// current, directed speed of the vehicle 
	float DirectedSpeed = 0.f;

	// current linear velocity 
	FVector Velocity = FVector::ZeroVector;

	// current linear velocity direction 
	FVector VelocityDirection = FVector::ForwardVector;

	// acceleration in local space 
	FVector AccelerationLocalSpace = FVector::ZeroVector;

	// acceleration world space 
	FVector AccelerationWorldSpace = FVector::ZeroVector;

	// angular velocity 
	// x = roll | y = pitch | z = yaw 
	FVector AngularVelocity = FVector::ZeroVector;
};


UCLASS()
class MORTALMOTORPROJECT_API ATurboBoost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurboBoost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateTurbo();

	// change camera position 
	void CameraPosition(float deltaTime);

	// reset camera position 
	void ResetCameraPosition();

private:

	APlayerMotorCar* PlayerCar;

	// timer variables 
	float MaxTurboDuration;
	float TurboTimer;

	// speed variables 
	float TurboAcceleration;
	float NormalizeScale;

	// camera variables 
	FVector OriginalSocketOffset; 
	float OriginalTargetArmLenght; 

	bool IsTurboActive;

	//Deactivates the turbo, reverting to normal acceleration 
	void DeactivateTurbo();

	// updates the turbo state and affects over time 
	void UpdateTurbo(float DeltaTime);

	// calculates and returns the adjusted acceleration 
	float CalculateTurboAcceleration();

	// adjust the maximum acceleration 
	void ModifyMaxAcceleration(float newMaxAcceleration);

	// adds a temporary boost to the vehicle 
	bool SpeedBoost(float amount, float duration, const FVector& direction);

	// performs conversion logic and return the speed in km/h 
	float ConvertCmPerSecToKmPerHour(float Speed);

protected:

	// Niagara FX variables 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UNiagaraComponent* TurboRightComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UNiagaraComponent* TurboLeftComp; 

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UArrowComponent* ArrowComp; 

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
		UAudioComponent* AudioComp; 

public:

	UPROPERTY(EditAnywhere, Category = TurboCar, meta = (UIMin = "0.1", UIMax = "2", ClampMin = "0.1", ClampMap = "10"))
		float Power;

	UPROPERTY(EditAnywhere, Category = TurboCar, meta = (UIMin = "0.1", UIMax = "2", ClampMin = "0.1", ClampMap = "10"))
		float Duration;

	UPROPERTY(EditAnywhere)
		FRuntimeFloatCurve BoostVsTime;

	// Structs variables

	FVehiclePropulsion Propulsion;

	FVehicleControl Control;

	FVehiclePhysics Physics;

	FPhysicsVelocityData PhysicsVel;

};
