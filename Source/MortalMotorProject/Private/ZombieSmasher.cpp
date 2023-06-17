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
 
}

void AZombieSmasher::BeginPlay()
{
	Super::BeginPlay();

    //AttackArea->OnComponentBeginOverlap.AddDynamic(this, &AZombieSmasher::OnAttackOverlap);
     
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
        bIsAttacking = false;       
    	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Zombie is chasing"));
    	ZombieController->MoveToLocation(TargetLocation);
    }
    else
    {
        ZombieController->StopMovement();
    }
}

 
void AZombieSmasher::Attack() const
{
    if(AttackAnims.Num() > 0)
    {
        int32 Random = FMath::RandRange(0, AttackAnims.Num() - 1);
        ZombieAnimInstance->Montage_Play(AttackAnims[Random]);
    }
    
} 

void AZombieSmasher::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    { 
        APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
        if (Car)
        {
            UPrimitiveComponent* CarRootComponent = Car->GetMesh();
            if (CarRootComponent && CarRootComponent->IsSimulatingPhysics())
            {
                Attack();
                const FVector RecoilDirection = this->GetActorForwardVector();
                const float RecoilForce = 200.0f;
                CarRootComponent->AddImpulse(RecoilDirection * RecoilForce, EName::None, true);
                Car->Health(AttackPower);
            }
        }
    }
}

//void AZombieSmasher::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//    if (OtherActor && OtherActor != this && OtherComp)
//    {
//       
//        APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
//        if (Car)
//        {
//        //     
//        //    // Apply a recoil to the mesh of the car in the opposite of the forward direction.
//        //    UPrimitiveComponent* CarRootComponent = Car->GetMesh();
//        //    if (CarRootComponent && CarRootComponent->IsSimulatingPhysics())
//        //    {
//        //        // const FVector RecoilDirection = -Car->GetActorForwardVector();
//        //        const FVector RecoilDirection = this->GetActorForwardVector();
//        //        const float RecoilForce = 500.0f;
//
//        //        if (!bIsAttacking)
//        //        {
//        //            bIsAttacking = true;
//        //            
//        //            CarRootComponent->AddImpulse(RecoilDirection * RecoilForce, EName::None, true);
//        //            Car->Health(AttackPower);
//
//        //            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("HIT BY ATTACK AREA"));
//        //        }
//        //    }
//            Attack();
//        }
//    }
//}


