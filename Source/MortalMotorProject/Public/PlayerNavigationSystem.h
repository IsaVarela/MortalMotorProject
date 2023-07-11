// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerNavigationSystem.generated.h"


class ADeliverTarget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MORTALMOTORPROJECT_API UPlayerNavigationSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerNavigationSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void ReachTarget(ADeliverTarget* DeliverTarget);

private:
	static ADeliverTarget* m_CurrentTarget;
	static TArray<AActor*> m_AllTargets;
	UStaticMeshComponent* m_TargetPointingArrow;
	class UTextRenderComponent* m_DistanceText;
	

	void FindTarget();
	void FindArrow();
	void FindDistanceText();
	void UpdateArrowRotation();
	void UpdateDistanceText();
	
};
