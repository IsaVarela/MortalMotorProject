// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

DECLARE_DELEGATE_OneParam(FOnEnemyKilledSignature, int /* goldAmount */)

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MORTALMOTORPROJECT_API IIDamageable
{
	GENERATED_BODY()

public:
	virtual void TakeDamge(float damage) = 0;
	virtual bool IsAlive() const = 0;

	virtual void ResetEnemy(FVector SpawnLocation) = 0;

	//Notifies the player that the enemy is dead
	static FOnEnemyKilledSignature OnEnemyKilledDelegate;
};
