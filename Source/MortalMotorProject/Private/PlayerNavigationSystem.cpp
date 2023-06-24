// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNavigationSystem.h"
#include "DeliverTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"

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

	if (m_Target == nullptr) { return; }

	UpdateArrowRotation();
	UpdateDistanceText();

}


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
	FVector direction = (m_Target->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();

	// Calculate the yaw rotation using trigonometry
	float Yaw = FMath::RadiansToDegrees(FMath::Atan2(direction.Y, direction.X));

	// Create the look rotation
	FRotator LookRotation(0.0f, Yaw, 0.0f);

	m_TargetPointingArrow->SetWorldRotation(LookRotation);

}

void UPlayerNavigationSystem::UpdateDistanceText()
{
	double distance = FVector::Dist(m_Target->GetActorLocation(), GetOwner()->GetActorLocation());
	distance /= 100;
	int32 flooredDistance = FMath::FloorToInt32(distance);
	
	m_DistanceText->SetText(FText::AsNumber(flooredDistance));
}



void UPlayerNavigationSystem::FindTarget()
{
	TArray<AActor*> CachedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Target"), CachedActors);

	if (CachedActors.Num() > 0)
	{
		ADeliverTarget* temp = Cast<ADeliverTarget>(CachedActors[0]);
		if (temp)
		{
			m_Target = temp;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong cast"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No target found"));
	}
}

