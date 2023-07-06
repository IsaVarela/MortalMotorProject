// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

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
}

void UGameOverWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGameOverWidget::LoadMainMenu()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelRef);
}

void UGameOverWidget::QuitGame()
{
	//FGenericPlatformMisc::RequestExit(false);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("QUIT HAS BEEN PRESSED"));
}
