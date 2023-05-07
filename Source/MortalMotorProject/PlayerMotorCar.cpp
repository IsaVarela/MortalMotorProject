// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotorCar.h"
#include "Gold.h"

APlayerMotorCar::APlayerMotorCar()
	:GoldAmount(0)
{
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);
}

void APlayerMotorCar::HandleGoldCollected()
{
	UE_LOG(LogTemp, Warning, TEXT("Gold Collected"));
	GoldAmount++;
}
