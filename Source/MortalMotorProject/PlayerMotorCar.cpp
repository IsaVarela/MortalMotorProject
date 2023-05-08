// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMotorCar.h"
#include "Gold.h"
#include "PlayerUI.h"

APlayerMotorCar::APlayerMotorCar()
	:GoldAmount(0)
{
	AGold::s_OnGoldCollected.BindUObject(this, &APlayerMotorCar::HandleGoldCollected);

}



void APlayerMotorCar::BeginPlay()
{
	Super::BeginPlay();

	PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), WidgetObject);
	PlayerUI->AddToViewport();
	
}

void APlayerMotorCar::HandleGoldCollected()
{
	GoldAmount++;
	OnGoldCollectedDelegate.ExecuteIfBound(GoldAmount);
}

