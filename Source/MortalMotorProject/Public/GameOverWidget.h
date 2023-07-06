// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	int SpecialZombie;
	int RegularZombie;
	float MilesTraveled;
	float TimeSurvived;
	float BestTime;

protected:
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;

private:
	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UWorld> LevelRef;

	UFUNCTION()
		void LoadMainMenu();
	UFUNCTION()
		void QuitGame();
};
