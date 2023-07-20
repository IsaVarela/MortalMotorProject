// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MotorGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API UMotorGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

public:
	UMotorGameInstance();

	UFUNCTION(BlueprintPure)
	float GetMasterVolume() const;

	UFUNCTION(BlueprintPure)
	float GetSFX_Volume() const;

	UFUNCTION(BlueprintPure)
	float GetMusic_Volume() const;

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float value);

	UFUNCTION(BlueprintCallable)
	void SetSFX_Volume(float value);

	UFUNCTION(BlueprintCallable)
		void SetMusic_Volume(float value); 
	
private:
	float m_masterVolume;
	float m_sfxVolume;
	float m_musicVolume; 
};
