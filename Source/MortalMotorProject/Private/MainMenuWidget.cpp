// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	UWidget* ButtonWidget = GetWidgetFromName(TEXT("StartButton"));

	if (ButtonWidget)
	{
		m_StartBtn = Cast<UButton>(ButtonWidget);

		if (m_StartBtn)
		{
			FScriptDelegate startButtonAction;
			startButtonAction.BindUFunction(this, FName("LoadGame"));
		
			m_StartBtn->OnClicked.Add(startButtonAction);
		}
	}

	

}

void UMainMenuWidget::BeginDestroy()
{
	Super::BeginDestroy();
	
}

void UMainMenuWidget::LoadGame()
{
	//UGameplayStatics::OpenLevel(this, FName("MainLevel"));
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelRef);
}
