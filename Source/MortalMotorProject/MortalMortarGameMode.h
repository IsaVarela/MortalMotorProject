// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreSkill.h"
#include "GameFramework/GameModeBase.h"
#include "MortalMortarGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API AMortalMortarGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

private:
	APlayerController* m_PlayerController;
	class APlayerMotorCar* m_Player;

	void InitAllCoreSkills();

	

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSkillUI();

	//Core Skills
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateMinigun();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateNitro();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateFlameThrower();

	UFUNCTION(BlueprintImplementableEvent)
	void ActivateMines();

	//Generic Skills
	UFUNCTION(BlueprintImplementableEvent)
	void GenericSkillTemp1();

	UFUNCTION(BlueprintImplementableEvent)
	void GenericSkillTemp2();

	UPROPERTY()
	TArray<FCoreSkill> m_AllCoreSkills;

	UPROPERTY()
	TArray<FCoreSkill> m_GenericSkills;

	FCoreSkill GetRandomSkill();

	void RemoveSelectedCoreSkill(const FCoreSkill& SkillToRemove);
	void AddSkillsToPool(const FCoreSkill& SkillToAdd);
};
