// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotorCar.h"
#include "Gold.h"
#include "PlayerUI.h"
#include "ChaosWheeledVehicleMovementComponent.h" 
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
 


APlayerMotorCar::APlayerMotorCar() :
	GoldAmount(0),
	Level(0)
{
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);
}

void APlayerMotorCar::BeginPlay()
{
	Super::BeginPlay();
	//GetMesh()->OnComponentHit.AddDynamic(this, &APlayerMotorCar::OnHit);
	//creat the Widget Ui based of the WidgetObject subclass
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), WidgetObject);
	//add the created UI to the viewport
	PlayerUI->AddToViewport();

	// Catch the spring arm component
	SpringArm = FindComponentByClass<USpringArmComponent>();

}

void APlayerMotorCar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Throttle: %f"), GetVehicleMovementComponent()->GetThrottleInput()));

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

void APlayerMotorCar::Accelerate()
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("FORWARD IS PRESSED"));
    GetVehicleMovementComponent()->SetThrottleInput(1);
}

void APlayerMotorCar::Break()
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("BREAK IS PRESSED"));
    GetVehicleMovement()->SetBrakeInput(1);
}

void APlayerMotorCar::Steer(float x)
{
    if (GetVehicleMovementComponent()->GetSteeringInput() != 0) {
        GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("Steer: %f"), GetVehicleMovementComponent()->GetSteeringInput()));
    }

    GetVehicleMovement()->SetSteeringInput(x);
}

//void APlayerMotorCar::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("iNSIDE FUNCTION"));
//	// Handle the hit event
//	if (OtherActor)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("WE GOT ANOTHER ACTOR"));
//		UE_LOG(LogTemp, Warning, TEXT("Hit event triggered with %s"), *OtherActor->GetName());
//	}
//}




 