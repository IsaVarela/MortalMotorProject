// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUI.h"
#include "PlayerMotorCar.h"
#include "PlayerNavigationSystem.h"
#include "Components/TextBlock.h"

//Upon creating the Widget
void UPlayerUI::NativeConstruct()
{
	 
	Super::NativeConstruct();
	 
	//Get the player and cache it
	Player = Cast<APlayerMotorCar>(GetOwningPlayer()->GetPawn());
	if (Player)
	{
		//Sub to the delegate when player collects the gold
		Player->OnGoldCollectedDelegate.BindUObject(this, &UPlayerUI::UpdateProgressBar);
	}

	//Get the Progress bar that was made in BP
	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressExp")));
	ProgressHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressHP")));
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, UPlayerNavigationSystem::FormattedTime);

	
}

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, UPlayerNavigationSystem::FormattedTime);
	UpdateTimer();
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

void UPlayerUI::UpdateHPBar(float val)
{
	if(ProgressHP)
	{
		val = FMath::Clamp(val, 0, 1);
		ProgressHP->SetPercent(val);
	}
}

void UPlayerUI::UpdateTimer()
{
	UTextBlock* GoalTimerText = Cast<UTextBlock>(GetWidgetFromName(TEXT("GoalTimer_Text")));

	if(GoalTimerText)
	{
		GoalTimerText->SetText(FText::FromString(UPlayerNavigationSystem::FormattedTime));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("no reference for text"));
	}
}
