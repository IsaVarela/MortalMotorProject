// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MortalMortarGameMode.generated.h"

struct FCoreSkill;

UCLASS()
class MORTALMOTORPROJECT_API AMortalMortarGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	class APlayerController* m_PlayerController;
	class APlayerMotorCar* m_Player;
	class UUpgradesComponent* m_UpgradeComponent;


	TArray<TSharedPtr<FCoreSkill>> m_AllCoreSkills;
	TArray<TSharedPtr<FCoreSkill>> m_GenericSkills;


	void InitAllCoreSkills();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSkillUI();

	//Core Skills
	UFUNCTION()
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

	TSharedPtr<FCoreSkill> GetRandomSkill();

	void RemoveSelectedCoreSkill(TSharedPtr <FCoreSkill> SkillToRemove);
	void AddSkillsToPool(TSharedPtr <FCoreSkill> SkillToAdd);
};
