// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MortalMotorProject/PlayerMotorCar.h"
#include "Kismet/GameplayStatics.h"

int UGameOverWidget::RegularZombieCount = 0;
int UGameOverWidget::SpecialZombieCount = 0;
int UGameOverWidget::TotalZombieCount = 0;
UGameOverWidget* UGameOverWidget::Instance = nullptr;

UGameOverWidget::UGameOverWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	 
	MilesTraveled = 0.0f;
	TimeSurvived = 0.0f;
	BestTime = 0.0f;
	Instance = this;
}

 void UGameOverWidget::IncrementRegularZombieCount()
{
	if(!APlayerMotorCar::bIsPlayerDead)
	{
		RegularZombieCount++;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("regular zombie : %i"), RegularZombieCount));

		if (Instance)
		{
			Instance->UpdateDisplay();
		}

		FinalZombieCount();
	}
	
}

void UGameOverWidget::IncrementSpecialZombieCount()
{
	if (!APlayerMotorCar::bIsPlayerDead)
	{
		SpecialZombieCount++;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Special zombie : %i"), SpecialZombieCount));

		if (Instance)
		{
			Instance->UpdateDisplay();
		}
		FinalZombieCount();
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
}

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UButton* RetryBtn = Cast<UButton>(GetWidgetFromName(TEXT("Retry_Btn")));
	UButton* QuitBtn = Cast<UButton>(GetWidgetFromName(TEXT("Quit_Btn")));
	//UTextBlock* RegularZombieCountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RegularZombies_Text")));

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
	RegularZombieCount = 0;
	SpecialZombieCount = 0;
	APlayerMotorCar::bIsPlayerDead = false;
	APlayerMotorCar::bResetCamera = true;
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelRef);
	 
}

void UGameOverWidget::QuitGame()
{
	//FGenericPlatformMisc::RequestExit(false);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("QUIT HAS BEEN PRESSED"));
}
