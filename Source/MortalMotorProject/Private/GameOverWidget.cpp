// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MortalMotorProject/PlayerMotorCar.h"
#include "Kismet/GameplayStatics.h"

int UGameOverWidget::RegularZombieCount;
int UGameOverWidget::SpecialZombieCount;
int UGameOverWidget::TotalZombieCount;
 

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 
	MilesTraveled = 0.0f;
	TimeSurvived = 0.0f;
	BestTime = 0.0f;	 
}

 void UGameOverWidget::IncrementRegularZombieCount()
{
	if(!APlayerMotorCar::bIsPlayerDead)
	{
		RegularZombieCount++;
		FinalZombieCount();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("regular zombie : %i"), RegularZombieCount));
 
	}
}

void UGameOverWidget::IncrementSpecialZombieCount()
{
	if (!APlayerMotorCar::bIsPlayerDead)
	{
		SpecialZombieCount++;
		FinalZombieCount();
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Special zombie : %i"), SpecialZombieCount));
 
	}
}

void UGameOverWidget::FinalZombieCount()
{
	TotalZombieCount = RegularZombieCount + SpecialZombieCount;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("TOTAL zombie : %i"), TotalZombieCount));
}



void UGameOverWidget::UpdateDisplay()
{
	UTextBlock* RegularZombieCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RegularZombies_Text")));
	UTextBlock* SpecialZombieCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SpecialZombies_Text")));
	UTextBlock* TotalZombieCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MonstersKilled_Text")));
	UTextBlock* SurvivedTimeCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SurvivedTime_Text")));
	UTextBlock* BestTimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("BestTime_Text")));

	if (RegularZombieCountText)
	{
		RegularZombieCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RegularZombieCount)));
	}

	if (SpecialZombieCountText)
	{
		SpecialZombieCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), SpecialZombieCount)));
	}

	if (TotalZombieCountText)
	{
		TotalZombieCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalZombieCount)));
	}

	if(SurvivedTimeCountText)
	{
		SurvivedTimeCountText->SetText(FText::FromString(APlayerMotorCar::SurvivedTime));
	}

	if (BestTimeText)
	{
		SurvivedTimeCountText->SetText(FText::FromString(APlayerMotorCar::BestTime));
	}
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UButton* RetryBtn = Cast<UButton>(GetWidgetFromName(TEXT("Retry_Btn")));
	UButton* QuitBtn = Cast<UButton>(GetWidgetFromName(TEXT("Quit_Btn")));
	 

	if (RetryBtn)
	{
		RetryBtn->OnClicked.AddDynamic(this, &UGameOverWidget::LoadMainMenu);
	}
	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UGameOverWidget::QuitGame);
	}
	 
	 
	UpdateDisplay();
}

void UGameOverWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

 

void UGameOverWidget::LoadMainMenu()
{
	RemoveFromParent();
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelRef);
	APlayerMotorCar::bResetCamera = true;
	RegularZombieCount = 0;
	SpecialZombieCount = 0;
	TotalZombieCount = 0;
}

void UGameOverWidget::QuitGame()
{
	//FGenericPlatformMisc::RequestExit(false);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("QUIT HAS BEEN PRESSED"));
}
