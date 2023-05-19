// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalMortarGameMode.h"
#include "PlayerMotorCar.h"

void AMortalMortarGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_PlayerController = GetWorld()->GetFirstPlayerController();

	if(m_PlayerController != nullptr)
	{
		m_Player = Cast<APlayerMotorCar>(m_PlayerController->GetPawn());

		if (m_Player)
		{
			m_Player->OnLevelUpDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateSkillUI);
		}
	}
}
