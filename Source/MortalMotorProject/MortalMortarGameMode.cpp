// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalMortarGameMode.h"
#include "PlayerMotorCar.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/Array.h"

void AMortalMortarGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);

	InitAllCoreSkills();
}

void AMortalMortarGameMode::BeginPlay()
{
	Super::BeginPlay();

	m_PlayerController = GetWorld()->GetFirstPlayerController();

	if (m_PlayerController != nullptr)
	{
		m_Player = Cast<APlayerMotorCar>(m_PlayerController->GetPawn());

		if (m_Player)
		{
			m_Player->OnLevelUpDelegate.AddUFunction(this, FName("ActivateSkillUI"));
		}
	}

}

void AMortalMortarGameMode::InitAllCoreSkills()
{
	//Create the skills
	CoreSkill_1 = FCoreSkill();
	CoreSkill_2 = FCoreSkill();
	CoreSkill_3 = FCoreSkill();
	//Assign names
	CoreSkill_1.SkillName = "MiniGun";
	CoreSkill_2.SkillName = "Nitro";
	CoreSkill_3.SkillName = "FlameThrower";

	//Finish Binding Delegates
	CoreSkill_1.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMinigun);
	CoreSkill_2.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateNitro);
	CoreSkill_3.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateFlameThrower);

	//Populate List
	m_AllCoreSkills.Add(CoreSkill_1);
	m_AllCoreSkills.Add(CoreSkill_2);
	m_AllCoreSkills.Add(CoreSkill_3);

}

FCoreSkill* AMortalMortarGameMode::GetRandomCoreSkillFromList()
{
	if (m_AllCoreSkills.IsEmpty()) { return nullptr; }
	int index = FMath::RandRange(0, (m_AllCoreSkills.Num() - 1)); //return a random object from the list

	return &m_AllCoreSkills[index];
}

void AMortalMortarGameMode::AddCoreSkillToList(const FCoreSkill& newSkill)
{
	m_AllCoreSkills.Add(newSkill);
}

void AMortalMortarGameMode::RemoveSelectedCoreSkill(FCoreSkill& SkillToRemove)
{
	if (m_AllCoreSkills.IsEmpty()) { return; }
	if (m_AllCoreSkills.Contains(SkillToRemove))
	{
		m_AllCoreSkills.RemoveSingle(SkillToRemove);
	}
}
