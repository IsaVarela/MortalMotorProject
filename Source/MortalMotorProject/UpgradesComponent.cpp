 // Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradesComponent.h"
#include "Minigun.h"
#include "LandMineSpawner.h"
#include "FlameThrower.h"
#include "TurboBoost.h"
#include "PlayerMotorCar.h"



// Sets default values for this component's properties
UUpgradesComponent::UUpgradesComponent():
	bIsShieldActive(false),
	m_healPercent(0.2f)
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
	auto temp = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Shield"));
	
	if (temp.Num() > 0)
	{
		m_shieldEnergy = Cast<UStaticMeshComponent>(temp[0]);
		
	}
}

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

void UUpgradesComponent::EnableEnergyShield()
{
	if (bIsShieldActive) { return; }
	if (m_shieldEnergy && m_player)
	{
		bIsShieldActive = true;

		if (DynamicMaterial == nullptr)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(m_shieldEnergy->GetMaterial(0), this);
		}
		
		// Set the opacity value (between 0.0 and 1.0) on the dynamic material
		float Opacity = 0.75f; // Set the desired opacity value
		DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), Opacity);

		// Assign the dynamic material to the mesh component
		m_shieldEnergy->SetMaterial(0, DynamicMaterial);

		m_player->ActivateShield(true);


		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("DeactivateShield"));
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.f, false,5.f);
		
	}
	
}

void UUpgradesComponent::DeactivateShield()
{
	m_player->ActivateShield(false);
	bIsShieldActive = false;

	float Opacity = 0.f;
	DynamicMaterial->SetScalarParameterValue(TEXT("Opacity"), Opacity);

	m_shieldEnergy->SetMaterial(0, DynamicMaterial);
}


