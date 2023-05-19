// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual void BeginPlay() override;

private:
	APlayerController* m_PlayerController;
	class APlayerMotorCar* m_Player;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ActivateSkillUI();
	
};
