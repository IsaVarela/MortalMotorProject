// Fill out your copyright notice in the Description page of Project Settings.


#include "MotorGameInstance.h"

void UMotorGameInstance::Init()
{
	Super::Init();

}

UMotorGameInstance::UMotorGameInstance():
	m_masterVolume(0),
	m_sfxVolume(0), 
	m_musicVolume(0)
{
	//UE_LOG(LogTemp, Warning, TEXT("TS"));
}

float UMotorGameInstance::GetMasterVolume() const
{
	return this->m_masterVolume;
}

float UMotorGameInstance::GetSFX_Volume() const
{
	return this->m_sfxVolume;
}

float UMotorGameInstance::GetMusic_Volume() const
{
	return this->m_musicVolume; 
}

void UMotorGameInstance::SetMasterVolume(float value)
{
	this->m_masterVolume = value;
}

void UMotorGameInstance::SetSFX_Volume(float value)
{
	this->m_sfxVolume = value;
}

void UMotorGameInstance::SetMusic_Volume(float value)
{
	this->m_musicVolume = value; 
}
