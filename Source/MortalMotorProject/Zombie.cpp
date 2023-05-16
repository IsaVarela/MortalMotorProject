// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"

AZombie::AZombie() 
{
	Skeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	Skeleton->SetupAttachment(RootComponent);
}