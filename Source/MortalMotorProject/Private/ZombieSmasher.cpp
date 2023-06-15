// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSmasher.h"

#include "Kismet/KismetMathLibrary.h"

AZombieSmasher::AZombieSmasher()
{
}

void AZombieSmasher::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieSmasher::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    PositionToPlayer();
    ChasePlayer(Player->GetActorLocation());
}

void AZombieSmasher::ChasePlayer(const FVector& TargetLocation) const
{
	 
    if (ZombieController && IsAlive())
    {
        const float DistanceToTarget = FVector::Dist(GetActorLocation(), TargetLocation);

        if (DistanceToTarget <= BlockingRadius && DotProduct > 0.7f)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie IS in front AND stopped"));
            Block();
        }

        if(DistanceToTarget <= AttackRadius && DotProduct <= 0.7f)
        {
            Attack();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie is chasing"));
            ZombieController->MoveToLocation(TargetLocation);
        }

        

    }
}

void AZombieSmasher::PositionToPlayer()
{
	const FVector zombiePosition = GetActorLocation();
    const FVector playerPosition = Player->GetActorLocation();
    const FVector playerForwardDirection = Player->GetActorForwardVector();
    const FVector playerToZombie = zombiePosition - playerPosition;

    // Calculate the dot product between the player's forward direction and the vector from player to zombie
     DotProduct = playerForwardDirection.GetSafeNormal().Dot(playerToZombie.GetSafeNormal());

     GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("DOT: %f"), DotProduct));

 
}

void AZombieSmasher::Attack() const
{
    if(AttackAnims.Num() > 0)
    {
        int32 Random = FMath::RandRange(0, AttackAnims.Num() - 1);
        ZombieAnimInstance->Montage_Play(AttackAnims[Random]);
    }
    
}

void AZombieSmasher::Block() const
{
    ZombieController->StopMovement();
}
