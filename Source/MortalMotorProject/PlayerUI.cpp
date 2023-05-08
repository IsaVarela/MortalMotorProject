// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "PlayerMotorCar.h"

void UPlayerUI::NativeConstruct()
{
	Player = Cast<APlayerMotorCar>(GetOwningPlayer()->GetPawn());
	if (Player)
	{
		Player->OnGoldCollectedDelegate.BindUObject(this, &UPlayerUI::UpdateProgressBar);
	}
}

void UPlayerUI::UpdateProgressBar(float val)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), val);
}
