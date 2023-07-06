// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotorCar.h"
#include "Gold.h"
#include "PlayerUI.h"
#include "ChaosWheeledVehicleMovementComponent.h" 
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "IDamageable.h"


APlayerMotorCar::APlayerMotorCar() :
	GoldAmount(0),
	Level(0),
	bIsInvinisible(false)
{
	//killzone sphere
	KillZoneCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Kill Zone Sphere"));
	KillZoneCollisionSphere->SetupAttachment(RootComponent);

	PlayerHealth = MAX_HEALTH;

}

void APlayerMotorCar::BeginPlay()
{
	Super::BeginPlay();
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);
	//create the Widget Ui based of the WidgetObject subclass
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), WidgetObject);
	//add the created UI to the viewport
	PlayerUI->AddToViewport();

	//cache the spring arm component
	SpringArm = Cast<USpringArmComponent>(GetComponentByClass(USpringArmComponent::StaticClass()));

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, FName("OnOverlapEnd"));
	KillZoneCollisionSphere->OnComponentEndOverlap.Add(ScriptDelegate);

	// get camera comp
	CameraComponent = this->FindComponentByClass<UCameraComponent>();
	
}

void APlayerMotorCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CameraRotation();

	if(CameraComponent)
	{
		if(bIsPlayerDead)
		{
			 
			// Calculate the new rotation based on the desired rotation amount
			FRotator NewRotation = SpringArm->GetComponentRotation() + (FRotator(0.0f, 10.0f, 0.0f) * DeltaSeconds);

			// Clamp the pitch and roll rotation values to a specific range
			float MinPitch = -90.0f;
			float MaxPitch = 0.0f;
			float MinRoll = -45.0f;
			float MaxRoll = 45.0f;
			NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, MinPitch, MaxPitch);
			NewRotation.Roll = FMath::Clamp(NewRotation.Roll, MinRoll, MaxRoll);

			// Apply the clamped rotation to the spring arm
			SpringArm->SetWorldRotation(NewRotation);
			//SpringArm->AddLocalRotation(FRotator(-5.0f, 10.0f, 0.0f) * DeltaSeconds);
		}
		
	}
 
}

void APlayerMotorCar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind different actions to the desired keys in the editor	
	PlayerInputComponent->BindAction("Forward", IE_Pressed, this, &APlayerMotorCar::Accelerate);
	PlayerInputComponent->BindAction("Break", IE_Pressed, this, &APlayerMotorCar::Break);
	PlayerInputComponent->BindAxis("Steer", this, &APlayerMotorCar::Steer);
}

//This function increments the gold and aslo retreives the the exp value that 
//The player should get for it. It increments the level if needed and send the info forward via the delegate
//To the UI to update the progress bar
void APlayerMotorCar::HandleGoldCollected()
{
	GoldAmount++;

	//Play SFX sound For gold collected
	
	if (GoldCollectSoundCue)
	{
		float startTime = FMath::RandRange(0.f, 1.f);
		UGameplayStatics::PlaySoundAtLocation(this, GoldCollectSoundCue, GetActorLocation(), 1.f, 1.f, startTime);
	}
	

	if (ExpCurveFloat == nullptr) { return; }
	float currentValue = ExpCurveFloat->GetFloatValue(GoldAmount);
	int levelTemp = Level;
	Level = FMath::FloorToInt(currentValue);

	//if the player leveled up
	if (Level > levelTemp)
	{
		OnLevelUpDelegate.Broadcast();
	}

	float finalValue = currentValue - Level;

	OnGoldCollectedDelegate.ExecuteIfBound(finalValue);
}

// this function will allow the player to rotate the camera without re adjusting to a default position
void APlayerMotorCar::CameraRotation()
{
    FRotator DefaultRotation;
    float LerpSpeed = 0.4f;
    float MouseX, MouseY;

    // Get the mouse movement delta from the player's input  
    GetWorld()->GetFirstPlayerController()->GetInputMouseDelta(MouseX, MouseY);

    {
        // Get the current rotation of the SpringArm
		FRotator CurrentRotation;
		if (SpringArm)
		{
			CurrentRotation = SpringArm->GetRelativeRotation();
		}
       

        // Update the Z rotation based on the mouse movement  
        float RotationSpeed = 0.8f;
        CurrentRotation.Yaw += MouseX * RotationSpeed;

        // Set the new rotation of the SpringArm
        SpringArm->SetRelativeRotation(CurrentRotation);
    }
}

 
void APlayerMotorCar::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		IIDamageable* Enemy = Cast<IIDamageable>(OtherActor);
		if (Enemy)
		{
			Enemy->TakeDamge(1000.f);
		}
		
	}
}

void APlayerMotorCar::Accelerate()
{
    GetVehicleMovementComponent()->SetThrottleInput(1);
}

void APlayerMotorCar::ActivateShield(bool state)
{
	bIsInvinisible = state;
}

void APlayerMotorCar::Break()
{
    GetVehicleMovement()->SetBrakeInput(1);
}

void APlayerMotorCar::Steer(float x)
{
    GetVehicleMovement()->SetSteeringInput(x);
}

void APlayerMotorCar::Health(float damage)
{
	if (bIsInvinisible) { return; }

	PlayerHealth = FMath::Max(0, PlayerHealth - damage);

	if (PlayerUI)
	{
		PlayerUI->UpdateHPBar(PlayerHealth / 100);
	}

	if (PlayerHealth == 0)
	{
		PlayerDead();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("HP: %f"), PlayerHealth));
}

void APlayerMotorCar::Heal(float amount)
{
	float healAmount = amount * MAX_HEALTH;
	PlayerHealth = FMath::Min(MAX_HEALTH, PlayerHealth + healAmount);

	if (PlayerUI)
	{
		PlayerUI->UpdateHPBar(PlayerHealth / 100);
	}

	
}

void APlayerMotorCar::PlayerDead()
{
	bIsPlayerDead = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PLAYER IS KAPUT"));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2f);
	//this->CustomTimeDilation = 0.5f;
	//UGameplayStatics::SetGamePaused(GetWorld(), true);
}



 