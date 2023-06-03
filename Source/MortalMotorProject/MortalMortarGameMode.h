// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MortalMortarGameMode.generated.h"

struct FCoreSkill;
class USkillsWidget;

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
	USkillsWidget* m_SkillWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USkillsWidget> SkillWidgetTemplate;

	int32 m_QueueCount = 0;
	bool bIsLevelUpInProgress = false;

	UFUNCTION()
	void IncrementQueueCount();
		

	TArray<TSharedPtr<FCoreSkill>> m_AllCoreSkills;
	TArray<TSharedPtr<FCoreSkill>> m_GenericSkills;

	UFUNCTION()
	void DisableSkillUI();

	UFUNCTION()
	void InitAllCoreSkills();

	UFUNCTION()
	void ActivateSkillUI();

public:
	

	//Core Skills
	UFUNCTION()
	void ActivateMinigun();

	UFUNCTION()
	void ActivateNitro();

	UFUNCTION()
	void ActivateFlameThrower();

	UFUNCTION()
	void ActivateMines();

	//Generic Skills
	UFUNCTION()
	void GenericSkillTemp1();

	UFUNCTION()
	void GenericSkillTemp2();

	TSharedPtr<FCoreSkill> GetRandomSkill();

	void RemoveSelectedCoreSkill(TSharedPtr <FCoreSkill> SkillToRemove);
	void AddSkillsToPool(TSharedPtr <FCoreSkill> SkillToAdd);
};
