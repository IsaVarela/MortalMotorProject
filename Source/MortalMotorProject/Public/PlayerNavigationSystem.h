// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerNavigationSystem.generated.h"


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

private:
	class ADeliverTarget* m_Target;
	UStaticMeshComponent* m_TargetPointingArrow;
	class UTextRenderComponent* m_DistanceText;

	void FindTarget();
	void FindArrow();
	void FindDistanceText();
	void UpdateArrowRotation();
	void UpdateDistanceText();
};
