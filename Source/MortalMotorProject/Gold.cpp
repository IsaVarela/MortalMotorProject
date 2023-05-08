// Fill out your copyright notice in the Description page of Project Settings.

#include "Gold.h"
#include "Components/TimelineComponent.h"

FOnGoldCollectedSignature AGold::s_OnGoldCollected;

// Sets default values
AGold::AGold()
{
	PrimaryActorTick.bCanEverTick = true;

	GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gold Mesh"));
	GoldMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGold::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimeLineProgressDel;

		TimeLineProgressDel.BindUFunction(this, FName("TimelineProgress"));
		CurveFTimeline.AddInterpFloat(CurveFloat, TimeLineProgressDel);
		CurveFTimeline.SetLooping(false);

		//Set the function to be called when the timeline finishes
		FOnTimelineEvent TimelineFinishedFunction;
		TimelineFinishedFunction.BindUFunction(this, FName("TimeLineFinish"));
		CurveFTimeline.SetTimelineFinishedFunc(TimelineFinishedFunction);
		
		StartLoc = GetActorLocation();
	}
	
}

// Called every frame
void AGold::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveFTimeline.TickTimeline(DeltaTime);

}

void AGold::OnGoldCollected()
{
	s_OnGoldCollected.ExecuteIfBound();
}

void AGold::AttractTowardsPlayer(AActor* playerActor)
{
	GoldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player = playerActor;
	CurveFTimeline.PlayFromStart();
}

void AGold::TimelineProgress(float val)
{
	if (Player)
	{
		FVector NewLocation = FMath::Lerp(StartLoc, Player->GetActorLocation(), val * AttractionSpeedMult);
		SetActorLocation(NewLocation);
	}
	
}

void AGold::TimeLineFinish()
{
	OnGoldCollected();
	Destroy();
}

