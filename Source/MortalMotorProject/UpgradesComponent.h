// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradesComponent.generated.h"

class AMinigun;
class ALandMineSpawner;
class AFlameThrower; 
class ATurboBoost; 
class APlayerMotorCar;


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
	AFlameThrower* m_FlameThrower; 
	ATurboBoost* m_TurboBoost; 
	APlayerMotorCar* m_player;
	UStaticMeshComponent* m_shieldEnergy;

	float m_healPercent;
	bool bIsShieldActive;
	UMaterialInstanceDynamic* DynamicMaterial;

	UFUNCTION()
	void DeactivateShield();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EnableMinigun();
	void EnableMines();
	void EnableFlameThrower(); 
	void EnableTurboBoost(); 
	void EnableGenericHeal();
	void EnableEnergyShield();
};
