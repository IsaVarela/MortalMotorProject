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
	 
	UGameOverWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "ZombieCount")
		static void IncrementRegularZombieCount();
	UFUNCTION(BlueprintCallable, Category = "ZombieCount")
		static void IncrementSpecialZombieCount();
	UFUNCTION(BlueprintCallable, Category = "ZombieCount")
		static void FinalZombieCount();
	void UpdateDisplay();

	static int32 RegularZombieCount;  
	static int32 SpecialZombieCount;
	static int32 TotalZombieCount;
	static UGameOverWidget* Instance;
	 

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
