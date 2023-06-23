// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSmasher.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "MortalMotorProject/PlayerMotorCar.h"

AZombieSmasher::AZombieSmasher()
{
    // Create the left arm collision component
    AttackArea = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
    AttackArea->SetupAttachment(GetMesh(), "AttackArea");

    bInAttackCollider = false;

}

void AZombieSmasher::BeginPlay()
{
	Super::BeginPlay();

    AttackArea->OnComponentBeginOverlap.AddDynamic(this, &AZombieSmasher::OnAttackOverlap);
    AttackArea->OnComponentEndOverlap.AddDynamic(this, &AZombieSmasher::OnAttackOverlapEnd);
     
}

void AZombieSmasher::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

        ChasePlayer(Player->GetActorLocation());
    
}

void AZombieSmasher::ChasePlayer(const FVector& TargetLocation) const
{
	 
    if (ZombieController && IsAlive() && !ZombieAnimInstance->IsAnyMontagePlaying())
    {
         
    	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie is chasing"));
    	ZombieController->MoveToLocation(TargetLocation);
    }
    else
    {
        ZombieController->StopMovement();
    }
}

 
void AZombieSmasher::PlayAttackAnim() const
{
     
    if(AttackAnims.Num() > 0)
    {
        int32 Random = FMath::RandRange(0, AttackAnims.Num() - 1);
        ZombieAnimInstance->Montage_Play(AttackAnims[Random]);
        
    }
    
}
 
  
void AZombieSmasher::AttackPlayer(AActor* OtherActor, float RecoilForce, float AttackDamage)
{
	APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
	if (Car)
	{
		UPrimitiveComponent* CarRootComponent = Car->GetMesh();
		if (CarRootComponent && CarRootComponent->IsSimulatingPhysics())
		{
			
			const FVector RecoilDirection = this->GetActorForwardVector();
			CarRootComponent->AddImpulse(RecoilDirection * RecoilForce, EName::None, true);
			Car->Health(AttackDamage);
		}
	}
}

void AZombieSmasher::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        AttackPlayer(OtherActor, 100.0f, 0.0f);
        PlayAttackAnim();
    }
}

void AZombieSmasher::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor && OtherActor != this && OtherComp)
    {
        
        APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
        if (Car)
        {
            bInAttackCollider = true;
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Reading from overlap boolean: %s"), bInAttackCollider ? TEXT("True") : TEXT("False")));
        }
    }
}

void AZombieSmasher::OnAttackOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
        if (Car)
        {
            bInAttackCollider = false;
            GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("LEFT ATTACK AREA"));
        }
    }
}


