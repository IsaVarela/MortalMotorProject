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
 


APlayerMotorCar::APlayerMotorCar() :
	GoldAmount(0),
	Level(0)
{
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);
	PlayerHealth = 100.0f;

	//killzone sphere
	KillZoneCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Kill Zone Sphere"));
	KillZoneCollisionSphere->SetupAttachment(RootComponent);
}

void APlayerMotorCar::BeginPlay()
{
	Super::BeginPlay();

	//create the Widget Ui based of the WidgetObject subclass
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), WidgetObject);
	//add the created UI to the viewport
	PlayerUI->AddToViewport();

	//cache the spring arm component
	SpringArm = FindComponentByClass<USpringArmComponent>();

	FScriptDelegate ScriptDelegate;
	ScriptDelegate.BindUFunction(this, FName("OnOverlapEnd"));
	KillZoneCollisionSphere->OnComponentEndOverlap.Add(ScriptDelegate);
}

void APlayerMotorCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CameraRotation();
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
        FRotator CurrentRotation = SpringArm->GetRelativeRotation();

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
		OtherActor->Destroy(); //this will make them disappear on impact. Need to cast it to IDamageable and take damage instead
	}
}

void APlayerMotorCar::Accelerate()
{
    GetVehicleMovementComponent()->SetThrottleInput(1);
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
	PlayerHealth = FMath::Max(0, PlayerHealth - damage);

	if (PlayerUI)
	{
		PlayerUI->UpdateHPBar(PlayerHealth / 100);
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("HP: %f"), PlayerHealth));
}





 