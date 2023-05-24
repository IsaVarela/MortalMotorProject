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

void AMortalMortarGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMortalMortarGameMode::InitAllCoreSkills()
{
	//Create Skills
	m_AllCoreSkills.Add(MakeShared<FCoreSkill>("MiniGun"));
	m_AllCoreSkills.Add(MakeShared<FCoreSkill>("Nitro"));
	m_AllCoreSkills.Add(MakeShared<FCoreSkill>("FlameThrower"));
	m_AllCoreSkills.Add(MakeShared<FCoreSkill>("Mines"));
	
	m_GenericSkills.Add(MakeShared<FCoreSkill>("Add 5% Speed"));
	m_GenericSkills.Add(MakeShared<FCoreSkill>("Add 5% HP"));


	
	//Finish Binding Delegates
	m_AllCoreSkills[0]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMinigun);
	m_AllCoreSkills[1]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateNitro);
	m_AllCoreSkills[2]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateFlameThrower);
	m_AllCoreSkills[3]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMines);

	m_GenericSkills[0]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp1);
	m_GenericSkills[1]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp2);
}

//----------------------Core Skill Implementations----------------------------
void AMortalMortarGameMode::ActivateMinigun()
{
	/*FInputModeGameOnly InputMode;
	m_PlayerController->SetInputMode(InputMode);
	m_PlayerController->bShowMouseCursor = false;*/
	
	m_UpgradeComponent->EnableMinigun();
}


TSharedPtr<FCoreSkill> AMortalMortarGameMode::GetRandomSkill()
{
	int index = -1;
	TSharedPtr<FCoreSkill> temp;

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


void AMortalMortarGameMode::RemoveSelectedCoreSkill(TSharedPtr<FCoreSkill> SkillToRemove)
{
	if (m_AllCoreSkills.IsEmpty()) { return; }
	
	if (m_AllCoreSkills.Contains(SkillToRemove))
	{
		m_AllCoreSkills.RemoveSingle(SkillToRemove);
	}
}

void AMortalMortarGameMode::AddSkillsToPool(TSharedPtr<FCoreSkill> SkillToAdd)
{
	m_AllCoreSkills.Add(SkillToAdd);
}
