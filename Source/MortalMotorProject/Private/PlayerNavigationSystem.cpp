// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNavigationSystem.h"
#include "DeliverTarget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include "MortalMotorProject/PlayerMotorCar.h"

ADeliverTarget* UPlayerNavigationSystem::m_CurrentTarget;
TArray<AActor*> UPlayerNavigationSystem::m_AllTargets;
int32 UPlayerNavigationSystem::flooredDistance;
float UPlayerNavigationSystem::TimeToTarget;
bool UPlayerNavigationSystem::IsTargetReached;
FString UPlayerNavigationSystem::FormattedTime;

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
	TimerToTarget(DeltaTime);
	if(IsTargetReached)
	{
		CalculateTimeToTarget();
		IsTargetReached = false;
	}
	 
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
	//int32 flooredDistance = FMath::FloorToInt32(distance);
	flooredDistance = FMath::FloorToInt32(distance);
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

	IsTargetReached = true;
}

void UPlayerNavigationSystem::CalculateTimeToTarget()
{
	float Speed = (30 * 1000) / 3600;
	TimeToTarget = flooredDistance / Speed;
	IsReady = true;
}

void UPlayerNavigationSystem::TimerToTarget(float DeltaTime)
{
 
	if (IsReady && TimeToTarget == 0.0f )
	{
		APlayerMotorCar* player = Cast<APlayerMotorCar>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (player)
		{
			 
			player->PlayerDead();
		}
	}
	  
	//TimeToTarget -= DeltaTime;
	TimeToTarget = FMath::Clamp(TimeToTarget - DeltaTime, 0, TimeToTarget);

	int32 Minutes = FMath::FloorToInt(TimeToTarget / 60);
	int32 Seconds = FMath::FloorToInt(TimeToTarget - (Minutes * 60));

	//FString FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FormattedTime);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), TimeToTarget));
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

	
	float Delay = 0.2f;
	FTimerHandle TimerHandleCount;
	GetOwner()->GetWorldTimerManager().ClearTimer(TimerHandleCount);
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandleCount, this, &UPlayerNavigationSystem::CalculateTimeToTarget, Delay, false);
}

