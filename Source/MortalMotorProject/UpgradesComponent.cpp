 // Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradesComponent.h"
#include "Minigun.h"
#include "LandMineSpawner.h"
#include "FlameThrower.h"
#include "TurboBoost.h"
#include "PlayerMotorCar.h"


// Sets default values for this component's properties
UUpgradesComponent::UUpgradesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

//Changes
// Called when the game starts
void UUpgradesComponent::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> ChildActors;
	GetOwner()->GetAllChildActors(ChildActors);

	//Here you can get all the actors that are children of the Player
	for (AActor* ChildActor : ChildActors)
	{
		if (ChildActor->IsA(AMinigun::StaticClass()))
		{
			m_Minigun = Cast<AMinigun>(ChildActor);
		}

		if (ChildActor->IsA(ALandMineSpawner::StaticClass()))
		{
			m_MinesSpawner = Cast<ALandMineSpawner>(ChildActor);
		}
		if (ChildActor->IsA(AFlameThrower::StaticClass()))
		{
			m_FlameThrower = Cast<AFlameThrower>(ChildActor); 
		}
		if (ChildActor->IsA(ATurboBoost::StaticClass()))
		{
			m_TurboBoost = Cast<ATurboBoost>(ChildActor); 
		}
	}

	m_player = Cast<APlayerMotorCar>(GetOwner());

	//if (m_player)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("WORKS"));
	//}
}


// Called every frame
void UUpgradesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UUpgradesComponent::EnableMinigun()
{
	if(m_Minigun != nullptr)
	{
		m_Minigun->ShowMinigun();
		m_Minigun->SetActorTickEnabled(true);
	}
}

void UUpgradesComponent::EnableMines()
{
	if (m_MinesSpawner != nullptr)
	{
		m_MinesSpawner->EnableLandMineSpawner();
	}
}

void UUpgradesComponent::EnableFlameThrower()
{
	if (m_FlameThrower != nullptr)
	{
		m_FlameThrower->ShowFlameThrower(); 
		m_FlameThrower->SetActorTickEnabled(true); 
	}
}

void UUpgradesComponent::EnableTurboBoost()
{
	if (m_TurboBoost != nullptr)
	{
		m_TurboBoost->ActivateTurbo(); 
		m_TurboBoost->SetActorTickEnabled(true); 
	}
}

void UUpgradesComponent::EnableGenericHeal()
{
	if (m_player)
	{
		m_player->Heal(m_healPercent);
	}
}

