// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttackNotify.h"
#include "ZombieSmasher.h"
#include "Engine.h"

void UEnemyAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	 
	if(MeshComp !=nullptr && MeshComp->GetOwner() != nullptr)
	{
		AZombieSmasher* Enemy = Cast<AZombieSmasher>(MeshComp->GetOwner());

		if (Enemy != nullptr && Enemy->bInAttackCollider)
		{
			 
			FString ActorName = Enemy->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Orange, FString::Printf(TEXT("Actor name: %s"), *ActorName));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("boolean: %s"), Enemy->bInAttackCollider ? TEXT("True") : TEXT("False")));			 
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("NOTIFY TRIGGERED ATTACK FUNCTION"));
			Enemy->AttackPlayer(Enemy->Player, 700.f, Enemy->AttackPower);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("NOTIFY DID NOT FIND A REFERENCE OR PLAYER LEFT COLLIDER"));
		}
	}
	
}

