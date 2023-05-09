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

	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressExp")));
}

void UPlayerUI::UpdateProgressBar(float val)
{
	if (ProgressBar)
	{
		val = FMath::Clamp(val, 0, 1);
		ProgressBar->SetPercent(val);
	}
}
