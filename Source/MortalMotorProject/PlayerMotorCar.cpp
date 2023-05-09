// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotorCar.h"
#include "Gold.h"
#include "PlayerUI.h"

APlayerMotorCar::APlayerMotorCar() :
	GoldAmount(0),
	Level(0)
{
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);

}

void APlayerMotorCar::BeginPlay()
{
	Super::BeginPlay();

	//creat the Widget Ui based of the WidgetObject subclass
	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), WidgetObject);
	//add the created UI to the viewport
	PlayerUI->AddToViewport();
	
}

//This function increments the gold and aslo retreives the the exp value that 
//The player should get for it. It increments the level if needed and send the info forward via the delegate
//To the UI to update the progress bar
void APlayerMotorCar::HandleGoldCollected()
{
	GoldAmount++;

	if (ExpCurveFloat == nullptr) { return; }
	float currentValue = ExpCurveFloat->GetFloatValue(GoldAmount);
	Level = FMath::FloorToInt(currentValue);
	float finalValue = currentValue - Level;

	OnGoldCollectedDelegate.ExecuteIfBound(finalValue);
}

