// Fill out your copyright notice in the Description page of Project Settings.


#include "MortalMortarGameMode.h"
#include "PlayerMotorCar.h"
#include "UpgradesComponent.h"
#include "CoreSkill.h"
#include "SkillsWidget.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.h"


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
			//m_Player->OnLevelUpDelegate.AddUFunction(this, FName("ActivateSkillUI"));
			m_Player->OnLevelUpDelegate.AddUFunction(this, FName("IncrementQueueCount"));

			m_UpgradeComponent = Cast<UUpgradesComponent>(m_Player->GetComponentByClass(UUpgradesComponent::StaticClass()));


			TArray<AActor*> OutArray;
			m_Player->GetAttachedActors(OutArray);

			//find the enemy spawner
			for (const auto& ele : OutArray)
			{
				m_EnemySpawner = Cast<AEnemySpawner>(ele);
				if (m_EnemySpawner){ break;}
			}

			if(m_EnemySpawner == nullptr){ UE_LOG(LogTemp, Warning, TEXT("NO ENEMY SPAWNER WAS FOUND")); }
		}
	}

	//Create Widget
	m_SkillWidget = CreateWidget<USkillsWidget>(m_PlayerController, SkillWidgetTemplate);
	m_SkillWidget->AddToViewport(100);
	m_SkillWidget->bIsFocusable = true;
	m_SkillWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	
	m_GenericSkills.Add(MakeShared<FCoreSkill>("Add 5% Speed",true));
	m_GenericSkills.Add(MakeShared<FCoreSkill>("Add 5% HP",true));


	
	//Finish Binding Delegates
	m_AllCoreSkills[0]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMinigun);
	m_AllCoreSkills[1]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateNitro);
	m_AllCoreSkills[2]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateFlameThrower);
	m_AllCoreSkills[3]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::ActivateMines);

	m_GenericSkills[0]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp1);
	m_GenericSkills[1]->OnSkillActionDelegate.BindUObject(this, &AMortalMortarGameMode::GenericSkillTemp2);
}

void AMortalMortarGameMode::IncrementQueueCount()
{
	m_QueueCount++;
	if (!bIsLevelUpInProgress)
	{
		ActivateSkillUI();
	}
}

void AMortalMortarGameMode::ActivateSkillUI()
{
	if (m_SkillWidget == nullptr) { return; }

	bIsLevelUpInProgress = true;
	m_SkillWidget->InitSkillChoices(); //generate new skills from the pool
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	m_SkillWidget->SetVisibility(ESlateVisibility::Visible);

	FInputModeUIOnly InputMode;
	m_PlayerController->SetInputMode(InputMode);
	m_PlayerController->bShowMouseCursor = true;
}

void AMortalMortarGameMode::DisableSkillUI()
{
	if (m_SkillWidget == nullptr) { return; }

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	FInputModeGameOnly InputMode;
	m_PlayerController->SetInputMode(InputMode);
	m_PlayerController->bShowMouseCursor = false;
	m_SkillWidget->SetVisibility(ESlateVisibility::Collapsed);
	m_QueueCount--;
	
	if (m_QueueCount > 0)
	{
		//Still skills in Queue goind inside activateskillUi again
		ActivateSkillUI();
	}

	else if (m_QueueCount == 0)
	{
		//no more skills in queue leaving
		bIsLevelUpInProgress = false;
	}
}

//----------------------Core Skill Implementations----------------------------
void AMortalMortarGameMode::ActivateMinigun()
{
	DisableSkillUI();
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

AEnemySpawner* AMortalMortarGameMode::GetEnemySpawner() const
{
	return m_EnemySpawner;
}

void AMortalMortarGameMode::ActivateNitro()
{
	DisableSkillUI();
	m_UpgradeComponent->EnableTurboBoost(); 
}

void AMortalMortarGameMode::ActivateFlameThrower()
{
	DisableSkillUI();
	m_UpgradeComponent->EnableFlameThrower(); 
}

void AMortalMortarGameMode::ActivateMines()
{
	m_UpgradeComponent->EnableMines();
	DisableSkillUI();
}

void AMortalMortarGameMode::GenericSkillTemp1()
{
	DisableSkillUI();
}

void AMortalMortarGameMode::GenericSkillTemp2()
{
	DisableSkillUI();
}
