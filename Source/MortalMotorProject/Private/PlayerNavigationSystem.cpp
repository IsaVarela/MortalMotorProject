// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNavigationSystem.h"
#include "DeliverTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

ADeliverTarget* UPlayerNavigationSystem::m_CurrentTarget;
TArray<AActor*> UPlayerNavigationSystem::m_AllTargets;

// Sets default values for this component's properties
UPlayerNavigationSystem::UPlayerNavigationSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerNavigationSystem::BeginPlay()
{
	Super::BeginPlay();

	FindTarget();
	FindArrow();
	FindDistanceText();
}

void UPlayerNavigationSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_CurrentTarget == nullptr) { return; }

	UpdateArrowRotation();
	UpdateDistanceText();

}

//gets hold of the actual arrow mesh
void UPlayerNavigationSystem::FindArrow()
{
	TArray<UActorComponent*> allActors = GetOwner()->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("Target_Arrow"));

	if (allActors.Num() > 0)
	{
		auto tempComp = Cast<UStaticMeshComponent>(allActors[0]);

		if (tempComp)
		{
			m_TargetPointingArrow = tempComp;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong cast arrow"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No arrow static mesh"));
	}
}

//gets hold of the text that displays the distance to target
void UPlayerNavigationSystem::FindDistanceText()
{
	TArray<UActorComponent*> allActors = GetOwner()->GetComponentsByTag(UTextRenderComponent::StaticClass(), FName("Distance_Text"));

	if (allActors.Num() > 0)
	{
		auto tempComp = Cast<UTextRenderComponent>(allActors[0]);

		if (tempComp)
		{
			m_DistanceText = tempComp;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong cast text render"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No text render found"));
	}
	
}

void UPlayerNavigationSystem::UpdateArrowRotation()
{
	if (m_CurrentTarget == nullptr) { return; }
	FVector direction = (m_CurrentTarget->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();

	// Calculate the yaw rotation using trigonometry
	float Yaw = FMath::RadiansToDegrees(FMath::Atan2(direction.Y, direction.X));

	// Create the look rotation
	FRotator LookRotation(0.0f, Yaw, 0.0f);

	m_TargetPointingArrow->SetWorldRotation(LookRotation);

}

void UPlayerNavigationSystem::UpdateDistanceText()
{
	if (m_CurrentTarget == nullptr) { return; }
	double distance = FVector::Dist(m_CurrentTarget->GetActorLocation(), GetOwner()->GetActorLocation());
	distance /= 100;
	int32 flooredDistance = FMath::FloorToInt32(distance);
	
	m_DistanceText->SetText(FText::AsNumber(flooredDistance));
}

void UPlayerNavigationSystem::ReachTarget(ADeliverTarget* DeliverTarget)
{
	DeliverTarget->DeactivateTarget();

	//select new target
	int randomIndex = FMath::RandRange(0, m_AllTargets.Num() - 1);
	m_CurrentTarget = Cast<ADeliverTarget>(m_AllTargets[randomIndex]);

	m_AllTargets.Add(DeliverTarget);
	m_AllTargets.Remove(m_CurrentTarget);

	m_CurrentTarget->ActivateTarget();
}



void UPlayerNavigationSystem::FindTarget()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeliverTarget::StaticClass(), m_AllTargets);
	
	if (m_AllTargets.Num() == 0) { UE_LOG(LogTemp, Warning, TEXT("Found NO TARGET!"));  return; }

	//UE_LOG(LogTemp, Warning, TEXT("Found %d"),m_AllTargets.Num());
	//Selects a random target to be the main target
	int randomIndex = FMath::RandRange(0, m_AllTargets.Num() - 1);


	m_CurrentTarget = Cast<ADeliverTarget>(m_AllTargets[randomIndex]);
	m_AllTargets.Remove(m_CurrentTarget);
	m_CurrentTarget->ActivateTarget();
}

