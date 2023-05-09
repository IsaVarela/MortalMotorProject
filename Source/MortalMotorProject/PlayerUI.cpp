// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "PlayerMotorCar.h"

//Upon creating the Widget
void UPlayerUI::NativeConstruct()
{
	//Get the player and cache it
	Player = Cast<APlayerMotorCar>(GetOwningPlayer()->GetPawn());
	if (Player)
	{
		//Sub to the delegate when player collects the gold
		Player->OnGoldCollectedDelegate.BindUObject(this, &UPlayerUI::UpdateProgressBar);
	}

	//Get the Progress bar that was made in BP
	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressExp")));
}

//Updates the progress bar based on value that was passed by the delegate
void UPlayerUI::UpdateProgressBar(float val)
{
	if (ProgressBar)
	{
		//Clamp the value for percent (0-100)
		val = FMath::Clamp(val, 0, 1);
		ProgressBar->SetPercent(val);
	}
}
