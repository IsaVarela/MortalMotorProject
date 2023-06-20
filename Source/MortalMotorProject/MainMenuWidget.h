// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class MORTALMOTORPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void BeginDestroy() override;

private:


	UButton* m_StartBtn;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LevelRef;

	UFUNCTION()
	void LoadGame();
	
};
