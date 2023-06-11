// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevel.h"
#include "MainMenuWidget.h"

void AMainMenuLevel::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Works"));
	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld()->GetFirstPlayerController(),MainMenuWidgetPrefab);
	MainMenuWidget->AddToViewport();
}