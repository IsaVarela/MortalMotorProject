// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Zombie.generated.h"

/**
 * 
 */
UCLASS()
class MORTALMOTORPROJECT_API AZombie : public AEnemy
{
	GENERATED_BODY()

public:

	AZombie();

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Skeleton;
	
};
