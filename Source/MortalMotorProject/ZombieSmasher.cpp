// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSmasher.h"

#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MortalMotorProject/PlayerMotorCar.h"

AZombieSmasher::AZombieSmasher()
{
    // Create the left arm collision component
    AttackArea = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
    AttackArea->SetupAttachment(GetMesh(), "AttackArea");

    bInAttackCollider = false;
    bIsSoundPlaying = false;

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
        AnimIndex = Random;
    }
    
}
 
  
void AZombieSmasher::AttackPlayer(AActor* OtherActor, float RecoilForce, float AttackDamage, bool PlaySound)
{
	APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
    AZombieRunner* Zombie = Cast<AZombieRunner>(OtherActor);
	if (Car)
	{
		UPrimitiveComponent* CarRootComponent = Car->GetMesh();
		if (CarRootComponent && CarRootComponent->IsSimulatingPhysics())
		{
			
			const FVector RecoilDirection = this->GetActorForwardVector();
			CarRootComponent->AddImpulse(RecoilDirection * RecoilForce, EName::None, true);
			Car->Health(AttackDamage);
        }

        if(PlaySound)// to play only when notify is called
        {
            if (AnimIndex == 0)
            {
                FTimerHandle TimerHandle;
                GetWorldTimerManager().SetTimer(TimerHandle, [this]() {
                    // Play the sound cue after the delay
                    UGameplayStatics::PlaySoundAtLocation(this, AttackingSound, GetActorLocation());
                    }, 0.3f, false);

                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT(" We are on anim 0000"));
            }
            else // to play when colliding or receiving punch 
            {
                UGameplayStatics::PlaySoundAtLocation(this, AttackingSound, GetActorLocation());
                //GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT(" We are on anim 111111"));
            }
        }
	}

    if(Zombie && Zombie->IsAlive())
    {
        Zombie->bIsCollidingWithPlayer = true;
        Zombie->TakeDamge(100);
        
        UPrimitiveComponent* ZombieRootComponent = Zombie->GetMesh();
        if(ZombieRootComponent && ZombieRootComponent->IsSimulatingPhysics())
        {
            // simple way 
            /*const FVector RecoilDirection = this->GetActorForwardVector();
            ZombieRootComponent->AddImpulse(RecoilDirection * 100.0f, EName::None, true);
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("SMASHER COLLIDING WITH ZOMBIE ADDING FORCE"));*/

            // experimental way 
            const FVector SmasherLocation = this->GetActorLocation();
            const FVector ZombieLocation = Zombie->GetActorLocation();
            const FVector SmasherToZombieDirection = (ZombieLocation - SmasherLocation).GetSafeNormal();
            const FVector ForwardVector = GetActorForwardVector();
            const FVector RightVector = GetActorRightVector();

           

            // Calculate the force direction based on the relative position of the smasher and the smaller zombie
            FVector ForceDirection = ForwardVector + RightVector;
            if (SmasherToZombieDirection.DotProduct(SmasherToZombieDirection,RightVector) < 0)
            {
                // Zombie is behind the smasher, change the force direction to forward-left
                ForceDirection = ForwardVector - RightVector;
            }

            // Add some height to the force direction
            const float HeightScale = 0.7f; //  add height
            ForceDirection += GetActorUpVector() * HeightScale;

            ZombieRootComponent->AddImpulse(ForceDirection * 6000.0f, NAME_None, true);
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("SMASHER YEETING"));
        }
    }
}

void AZombieSmasher::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherComp)
    {
        if(OtherActor == Player)
        {
            PlayAttackAnim();
            AudioComponent->SetSound(AttackingDefault);
            AudioComponent->Play();
        }
        AttackPlayer(OtherActor, 100.0f, 0.0f, false);

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
            //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, FString::Printf(TEXT("Reading from overlap boolean: %s"), bInAttackCollider ? TEXT("True") : TEXT("False")));
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
           // GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Red, TEXT("LEFT ATTACK AREA"));
        }
    }
}


