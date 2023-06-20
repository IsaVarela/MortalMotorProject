// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevel.generated.h"

class UMainMenuWidget;
UCLASS()
class MORTALMOTORPROJECT_API AMainMenuLevel : public ALevelScriptActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetPrefab;
	
};
