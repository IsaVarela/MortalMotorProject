// Fill out your copyright notice in the Description page of Project Settings.


#include "TurboBoost.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "PlayerMotorCar.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Logging/LogMacros.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATurboBoost::ATurboBoost() : 
	IsTurboActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);

	TurboRightComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TurboRight"));
	TurboRightComp->bAutoActivate = false;
	TurboRightComp->SetupAttachment(ArrowComp);

	TurboLeftComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TurboLeft"));
	TurboLeftComp->bAutoActivate = false;
	TurboLeftComp->SetupAttachment(ArrowComp);

	MaxTurboDuration = 5.f;
	TurboAcceleration = 2000.f;
	TurboTimer = 0.f;
	NormalizeScale = 1.f;

	Power = 2000.f;
	Duration = 5.f;

	OriginalSocketOffset = FVector(0.f, 0.f, 600.f);
	OriginalTargetArmLenght = 700.f;

	BoostVsTime.GetRichCurve()->AddKey(0, 0.f);
	BoostVsTime.GetRichCurve()->AddKey(1, 0.2f);
	BoostVsTime.GetRichCurve()->AddKey(2, 0.2f);
	BoostVsTime.GetRichCurve()->AddKey(3, 0.1f);
	BoostVsTime.GetRichCurve()->AddKey(4, 0.05f);
	BoostVsTime.GetRichCurve()->AddKey(5, 0.f);

	PlayerCar = Cast<APlayerMotorCar>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerMotorCar::StaticClass()));
}

// Called when the game starts or when spawned
void ATurboBoost::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ATurboBoost::Tick(float DeltaTime)
{
	if (!IsTurboActive)
		return;

	Super::Tick(DeltaTime);

	float CurrentVelocity = PlayerCar->GetVehicleMovementComponent()->GetForwardSpeed();

	//UE_LOG(LogTemp, Warning, TEXT("PlayerCar VelocityBeginig: %f"), CurrentVelocity);

	UpdateTurbo(DeltaTime);

	if(IsTurboActive)
	{
		// change camera position 
		CameraPosition(DeltaTime);

		// calculate turbo acceleration 
		float turboAcceleration = CalculateTurboAcceleration();

		SpeedBoost(turboAcceleration, MaxTurboDuration, FVector::ForwardVector);
		ModifyMaxAcceleration(TurboAcceleration);
		ConvertCmPerSecToKmPerHour(TurboAcceleration);

		//UE_LOG(LogTemp, Warning, TEXT("PlayerCar Velocity: %f"), CurrentVelocity);
	}

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATurboBoost::DeactivateTurbo, MaxTurboDuration, false);
}

void ATurboBoost::ActivateTurbo()
{
	IsTurboActive = true;
}

void ATurboBoost::CameraPosition(float deltaTime)
{
	if (PlayerCar->SpringArm)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CameraPosition Change"));
		// modify the SocketOffset
		FVector ModifySocketOffset = FVector(0.f, 0.f, 350.f);

		PlayerCar->SpringArm->SocketOffset = FMath::Lerp(PlayerCar->SpringArm->SocketOffset, ModifySocketOffset, deltaTime * 0.6f);
		
		//PlayerCar->SpringArm->SocketOffset = ModifySocketOffset;

		// modify the TargetArmLength 
		float ModifyTargetArmLength = 400.f;
		//PlayerCar->SpringArm->TargetArmLength = ModifyTargetArmLength;
		PlayerCar->SpringArm->TargetArmLength = FMath::Lerp(PlayerCar->SpringArm->TargetArmLength, ModifyTargetArmLength, deltaTime * 0.6f);
	}
}

void ATurboBoost::ResetCameraPosition()
{
	if (PlayerCar->SpringArm)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("CameraPosition Reset"));*/
		PlayerCar->SpringArm->SocketOffset = OriginalSocketOffset;
		PlayerCar->SpringArm->TargetArmLength = OriginalTargetArmLenght;
	}
}

void ATurboBoost::DeactivateTurbo()
{
	//UE_LOG(LogTemp, Warning, TEXT("Deavitvate Turbo working? "));
	TurboTimer = 0.f;

	IsTurboActive = false;

	// revert vehicles acceleration to its original value 
	//ModifyMaxAcceleration(
		
	PlayerCar->GetVehicleMovementComponent()->SetThrottleInput(1); 

	Control.ThrottleInput = Control.RawThrottleInput; 

	// Deactivate niagara fx 
	//UE_LOG(LogTemp, Warning, TEXT("Deactivate Niagara"));
	TurboRightComp->Deactivate();
	TurboLeftComp->Deactivate();

	ResetCameraPosition();
}

void ATurboBoost::UpdateTurbo(float DeltaTime)
{
	if (IsTurboActive)
	{
		//UE_LOG(LogTemp, Warning, TEXT("TurboTimer before increment: %f"), TurboTimer);
		TurboTimer += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("TurboTimer after increment: %f"), TurboTimer);

		// check if the turbo duration has passed 
		if (TurboTimer >= MaxTurboDuration)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Turbo timer has reached max duration"));
			DeactivateTurbo();
		}
	}
}

float ATurboBoost::CalculateTurboAcceleration()
{
	// calculate the current boost scale 
	Propulsion.BoostScale = BoostVsTime.GetRichCurve()->Eval(TurboTimer / MaxTurboDuration);

	// calculate the turbo acceleration based on the turbo scale and the turbo acceleration 
	float turboAcceleration = TurboAcceleration * Propulsion.BoostScale;

	// access relevant properties from vehicle control 
	float throttleInput = Control.RawThrottleInput;
	float brakePosition = Control.BrakePosition;

	// apply throttle and brake adjustments to turbo acceleration 
	turboAcceleration *= throttleInput * (1.f - brakePosition);

	float KmPerHourturboAcceleration = ConvertCmPerSecToKmPerHour(turboAcceleration);

	return KmPerHourturboAcceleration;
}

void ATurboBoost::ModifyMaxAcceleration(float newMaxAcceleration)
{
	// adjust the max acceleration based on the engine power 
	Propulsion.MaxJetEnginePower = newMaxAcceleration * Power;

	// update throttle input 
	if (Control.ThrottleInput > Propulsion.MaxJetEnginePower)
	{
		Control.ThrottleInput = Propulsion.MaxJetEnginePower;
	}
}

bool ATurboBoost::SpeedBoost(float amount, float duration, const FVector& direction)
{
	// apply the boost by modifying the vehicles propulsion properties 
	Propulsion.Boost += amount;
	Propulsion.CurrentJetEnginePower = Propulsion.MaxJetEnginePower;
	Propulsion.TurboThrottle = 1.f;

	// set turbo duration and initialize the timer 
	MaxTurboDuration = 5.f;
	TurboTimer = 0.f;
	IsTurboActive = true;

	// calculate linear velocity using the safe direction and maximum speed of the vehicle 
	FVector safeDirection = direction.GetSafeNormal();
	float maxSpeed = PlayerCar->GetVehicleMovementComponent()->GetForwardSpeed();
	Propulsion.BoostScale = BoostVsTime.GetRichCurve()->Eval(TurboTimer / MaxTurboDuration);

	FVector linearVelocity = (safeDirection * maxSpeed) + (safeDirection * amount) * Power;
	FVector angularVelocity = FVector::ZeroVector;
	FVector xDirection = FVector::ForwardVector;

	// apply the boosts direction to the vehicle physics 
	PhysicsVel.SetVelocities(linearVelocity, angularVelocity, xDirection);

	// activate niagara fx 
	//UE_LOG(LogTemp, Warning, TEXT("Niagara Active"));
	TurboRightComp->Activate();
	TurboLeftComp->Activate();

	PlayerCar->Accelerate();

	return true;
}

float ATurboBoost::ConvertCmPerSecToKmPerHour(float Speed)
{
	const float cmToKm = 0.00001f;
	const float secToHour = 3600.f;

	return Speed * cmToKm * secToHour;
}

void FPhysicsVelocityData::SetVelocities(const FVector& linearVelocity, const FVector& angularVelocity, const FVector& xDirection)
{
	Velocity = linearVelocity;
	VelocityDirection = Velocity;

	if (VelocityDirection.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		VelocityDirection = xDirection;
	}
	else
	{
		VelocityDirection.Normalize();
	}

	Speed = Velocity.Size();

	// set max Speed
	float maxSpeed = 5555.55f;

	if (Speed > maxSpeed)
	{
		Speed = maxSpeed;
		Velocity = VelocityDirection * Speed;
	}

	DirectedSpeed = Speed;
	if (Speed < 10.f)
	{
		DirectedSpeed *= FVector::DotProduct(xDirection, VelocityDirection) * Speed;
	}
}
