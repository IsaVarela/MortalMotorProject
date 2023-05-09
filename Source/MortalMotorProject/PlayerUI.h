// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	class APlayerMotorCar* Player;

	void UpdateProgressBar(float val);

	UProgressBar* ProgressBar;

	
};
