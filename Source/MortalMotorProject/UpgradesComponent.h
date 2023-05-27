// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradesComponent.generated.h"

class AMinigun;
class ALandMineSpawner;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MORTALMOTORPROJECT_API UUpgradesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AMinigun* m_Minigun;
	ALandMineSpawner* m_MinesSpawner;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EnableMinigun();
	void EnableMines();
};
