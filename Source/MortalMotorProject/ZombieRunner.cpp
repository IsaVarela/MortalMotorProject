// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieRunner.h"
#include "AIController.h" 
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMotorCar.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "MortalMortarGameMode.h"
#include "EnemySpawner.h"
#include "Engine/CollisionProfile.h"

// Sets default values
AZombieRunner::AZombieRunner()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackPower = 5.0f;

	//Get particle system
	bIsPsPlaying = false;
	HitParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffectComponent"));
	HitParticlesComponent->SetupAttachment(RootComponent);
	HitParticlesComponent->bAutoActivate = false;
	Template_Blood = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/Juan_Active_Branch/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	Template_Fire = LoadObject<UParticleSystem>(nullptr, TEXT("/Script/Engine.ParticleSystem'/Game/AssetPacks/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/P_Fire_Big.P_Fire_Big'"));

	//Get audio
	bIsSoundPlaying = false;
	SoundCueBodyFall = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Enemies/SFX/SC_Body_Fall.SC_Body_Fall'"));
	SoundCueHitCar = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Enemies/SFX/SC_Car_Hit.SC_Car_Hit'"));
	SoundCueApproach = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Blueprints/Enemies/SFX/Zombie_Approach_Cue.Zombie_Approach_Cue'"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetRootComponent());
	AudioComponent->bAutoActivate = false;
}

void AZombieRunner::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// after the constructor is completed check if the components were added and start assigning
	if (GetMesh())
	{
		ZombieAnimInstance = GetMesh()->GetAnimInstance();
	}

		if (Hit_Montage01)
	{
		Hit_Montages.Add(Hit_Montage01);
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ERROR Hit_Montage01 NOT LOADED"));
	}

	if (Hit_Montage02)
	{
		Hit_Montages.Add(Hit_Montage02);
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ERROR Hit_Montage02 NOT LOADED"));
	}

	if (Death_Montage01)
	{
		Death_Montages.Add(Death_Montage01);
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ERROR Death_Montage01 NOT LOADED"));
	}

	if (Death_Montage02)
	{
		Death_Montages.Add(Death_Montage02);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("ERROR Death_Montage02 NOT LOADED"));
	}

}

// Called when the game starts or when spawned
void AZombieRunner::BeginPlay()
{
	Super::BeginPlay();

	ZombieController = Cast<AAIController>(GetController());

	Player = UGameplayStatics::GetPlayerPawn(this, 0);

	bIsCollidingWithPlayer = false;

	bIsBurned = false;

	this->GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AZombieRunner::OnOverlapBegin);

}

// Called every frame
void AZombieRunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 

	if (Player)
	{
		ChasePlayer(Player->GetActorLocation());
	}
}


void AZombieRunner::TakeDamge(float damage)
{

	HealthPoints = FMath::Max(0, HealthPoints - damage);
	// rewrote this part to use the IsAlive function since it already returns the hitpoint count
	if (IsAlive())
	{
		// playing a random index from an array that contains the animations when the zombie is hit by something
		if (Hit_Montages.Num() > 0 && ZombieAnimInstance)
		{
			int32 RandomIndex = FMath::RandRange(0, Hit_Montages.Num() - 1);
			ZombieAnimInstance->Montage_Play(Hit_Montages[RandomIndex], 1.0f);
			 
		 
		}

		if (HitParticlesComponent)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("PARTICLE EFFECTS PLAY"));
			ParticleSystem();
		}
	}
	else
	{
		Death();
	}
}

void AZombieRunner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		APlayerMotorCar* Car = Cast<APlayerMotorCar>(OtherActor);
		if (Car)
		{
			bIsCollidingWithPlayer = true;
			// Print collision for debugging
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Zombie collided with Car: %s"), bIsCollidingWithPlayer ? TEXT("true") : TEXT("false")));
			TakeDamge(100.0f);
			Car->Health(AttackPower);
		}

	}
}

void AZombieRunner::ChasePlayer(const FVector& TargetLocation) const
{
	
	if (ZombieController && IsAlive())
	{
		ZombieController->MoveToLocation(TargetLocation);

		if (!bIsSoundPlaying)
		{

			if (AudioComponent)
			{
				AudioComponent->SetSound(SoundCueApproach);					 
				bIsSoundPlaying = true;
				AudioComponent->Play();
			}
		}
	}
	else
	{
		if(AudioComponent)
		AudioComponent->Stop();		 
	}

}
 
void AZombieRunner::Death()
{
	DisableCollision();

	//command assigned directly to the character movement component it halts the object regardless of the AIController.  
	this->GetCharacterMovement()->StopMovementImmediately();
	

	if (bIsCollidingWithPlayer)
	{
		if (HitParticlesComponent)
		{
			ParticleSystem();		 
		}

		//BECOME RAGDOLL
		BecomeRagdoll();

		if(SoundCueBodyFall)
		{
			//float Cue1duration = SoundCueHitCar->Duration;  // should use some way of collision detection instead of time
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AZombieRunner::PlaySoundCueHitGround, 1.0f, false);
		}
	}
	else
	{
		if (Death_Montages.Num() > 0 && ZombieAnimInstance)
		{
			const int32 RandomIndex = FMath::RandRange(0, Death_Montages.Num() - 1);
			ZombieAnimInstance->Montage_Play(Death_Montages[RandomIndex], 1.0f);
		}
	}

	//Delay before Hiding the enemy
	float Delay = 4.0f;
	FTimerHandle TimerHandle_DeathCount;
	GetWorldTimerManager().SetTimer(TimerHandle_DeathCount, this, &AZombieRunner::KillEnemy, Delay, false);
}

//This function was added to replay the PS every time it is called instead of waiting for it to finish its regular lifetime before calling again
void AZombieRunner::ParticleSystem()
{
	if (!bIsBurned)
	{
		if (!bIsPsPlaying)
		{
			// add the template and play ps
			HitParticlesComponent->SetTemplate(Template_Blood);
			HitParticlesComponent->ActivateSystem();

			// Set bool to true
			bIsPsPlaying = true;
		}
		else
		{
			// Stop the currently playing instance
			HitParticlesComponent->DeactivateSystem();

			// Spawn a new particle system instance
			HitParticlesComponent->SetTemplate(Template_Blood);
			HitParticlesComponent->ActivateSystem();
		}
	} else
	{
		if (!bIsPsPlaying)
		{
			// add the template and play ps
			HitParticlesComponent->SetTemplate(Template_Fire);
			HitParticlesComponent->ActivateSystem();

			// Set bool to true
			bIsPsPlaying = true;
		}
		else
		{
			// Stop the currently playing instance
			HitParticlesComponent->DeactivateSystem();

			// Spawn a new particle system instance
			HitParticlesComponent->SetTemplate(Template_Fire);
			HitParticlesComponent->ActivateSystem();
		}
	}

}

void AZombieRunner::DisableCollision()
{
	this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZombieRunner::ResetEnemy()
{
	this->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//this->GetCharacterMovement()->SetMovementMode(MOVE_Walking);  //-> this doesnt appear to be doing anything
	 
	SetActorHiddenInGame(false);
	HealthPoints = 100.f; //or any max health per zombie type;

	if (bIsCollidingWithPlayer)
	{
		const ECollisionChannel CollisionObjectType = ECC_GameTraceChannel3; // this is the trace channel associated with Enemy collision type
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionObjectType(CollisionObjectType);

		// to get the mesh to follow the capsule again, it was necessary to refresh the attachment of the mesh to the capsule while maintaining the original stored identity transform
		// otherwise the mesh keeps duplicating in size
		GetMesh()->SetRelativeTransform(FTransform::Identity);
		GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		// it is necessary to manually add the offsets in rotation, scale and location again after reattachment otherwise the mesh will float above with default values. 
		FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);   
		FVector NewRelativeLocation = FVector(-25.0f, 0.0f, -90.0f);   
		
		// Set values for the mesh
		GetMesh()->SetRelativeRotation(NewRotation);
		GetMesh()->SetRelativeLocation(NewRelativeLocation);
		GetMesh()->SetRelativeScale3D(NewSize); // new size is public since each zombie variant has a different mesh size adjust in bp 
		bIsCollidingWithPlayer = false;
	} 
		 
	 else
	{
		if(ZombieAnimInstance)
		ZombieAnimInstance->Montage_Stop(0, nullptr);
	}
}


// set the zombie to ragdoll collision type and set simulate physics to true 
void AZombieRunner::BecomeRagdoll()
{

	const ECollisionChannel CollisionObjectType = ECC_PhysicsBody;
	GetMesh()->SetCollisionObjectType(CollisionObjectType);
	GetMesh()->SetSimulatePhysics(true);
	//DisableCollision();
 
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCueHitCar, this->GetActorLocation());
	}
}

void AZombieRunner::KillEnemy()
{

	//PUT ENEMY IN POOL
	AMortalMortarGameMode* MortalGameMode = Cast<AMortalMortarGameMode>(GetWorld()->GetAuthGameMode());
	if (MortalGameMode)
	{
		SetActorHiddenInGame(true);
		MortalGameMode->GetEnemySpawner()->PutEnemyBackInThePool(this);
	}

	ResetEnemy();
	
}

void AZombieRunner::PlaySoundCueHitGround()
{
	if(SoundCueBodyFall)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCueBodyFall, this->GetActorLocation());
	}
}




