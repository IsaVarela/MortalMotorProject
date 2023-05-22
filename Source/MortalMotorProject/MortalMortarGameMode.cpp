// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalMortarGameMode.h"
#include "PlayerMotorCar.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/Array.h"
#include "UpgradesComponent.h"
#include "CoreSkill.h"


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

			m_UpgradeComponent = Cast<UUpgradesComponent>(m_Player->GetComponentByClass(UUpgradesComponent::StaticClass()));
		}
	}

	
}

void AMortalMortarGameMode::InitAllCoreSkills()
{
	//Create the skills
	FCoreSkill CoreSkill_1;
	FCoreSkill CoreSkill_2;
	FCoreSkill CoreSkill_3;
	FCoreSkill CoreSkill_4;

	//Generic Skills
	FCoreSkill GenericSkill_1;
	FCoreSkill GenericSkill_2;

	GenericSkill_1.SkillName = "Add 5% Speed";
	GenericSkill_2.SkillName = "Add 5% HP";

	//Assign names
	CoreSkill_1.SkillName = "MiniGun";
	CoreSkill_2.SkillName = "Nitro";
	CoreSkill_3.SkillName = "FlameThrower";
	CoreSkill_4.SkillName = "Mines";

	//Finish Binding Delegates
	CoreSkill_1.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMinigun);
	CoreSkill_2.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateNitro);
	CoreSkill_3.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateFlameThrower);
	CoreSkill_4.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMines);

	GenericSkill_1.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp1);
	GenericSkill_2.OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp2);

	
	//Populate List
	m_AllCoreSkills.Add(CoreSkill_1);
	m_AllCoreSkills.Add(CoreSkill_2);
	m_AllCoreSkills.Add(CoreSkill_3);
	m_AllCoreSkills.Add(CoreSkill_4);

	m_GenericSkills.Add(GenericSkill_1);
	m_GenericSkills.Add(GenericSkill_2);
}

//----------------------Core Skill Implementations----------------------------
void AMortalMortarGameMode::ActivateMinigun()
{
	/*FInputModeGameOnly InputMode;
	m_PlayerController->SetInputMode(InputMode);
	m_PlayerController->bShowMouseCursor = false;*/
	
	m_UpgradeComponent->EnableMinigun();
}


FCoreSkill AMortalMortarGameMode::GetRandomSkill()
{
	int index = -1;
	FCoreSkill temp;

	if (!m_AllCoreSkills.IsEmpty())
	{
		index = FMath::RandRange(0, (m_AllCoreSkills.Num() - 1));
		temp = m_AllCoreSkills[index];
		RemoveSelectedCoreSkill(temp);
	}
	else 
	{
		index = FMath::RandRange(0, (m_GenericSkills.Num() - 1));
		temp = m_GenericSkills[index];
	}
	
	return temp;
}


void AMortalMortarGameMode::RemoveSelectedCoreSkill(const FCoreSkill& SkillToRemove)
{
	if (m_AllCoreSkills.IsEmpty()) { return; }
	
	if (m_AllCoreSkills.Contains(SkillToRemove))
	{
		m_AllCoreSkills.RemoveSingle(SkillToRemove);
	}
}

void AMortalMortarGameMode::AddSkillsToPool(const FCoreSkill& SkillToAdd)
{
	m_AllCoreSkills.Add(SkillToAdd);
}
