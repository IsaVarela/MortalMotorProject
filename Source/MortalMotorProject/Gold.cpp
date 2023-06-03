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

	//The curve is the fashion of movement towards the player
	if (CurveFloat)
	{
		//Declaring all the timeline params
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

	//init the curveTimeline with the time value (how it counts)
	CurveFTimeline.TickTimeline(DeltaTime);

}

void AGold::OnGoldCollected()
{
	//A static delegate get called when the gold finishes it's timeline
	s_OnGoldCollected.ExecuteIfBound();
}

void AGold::AttractTowardsPlayer(AActor* playerActor)
{
	//disable collision to avoid getting hit by the player's sphere cast again
	GoldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Player = playerActor;
	CurveFTimeline.PlayFromStart();
}

void AGold::TimelineProgress(float val)
{
	if (Player)
	{
		//Interpolate the gold pos to the player pos
		FVector NewLocation = FMath::Lerp(StartLoc, Player->GetActorLocation(), val * AttractionSpeedMult);
		SetActorLocation(NewLocation);
	}
	
}

void AGold::TimeLineFinish()
{
	//When the timeline ends, notify the player that the gold was collected and destroy this object
	OnGoldCollected();
	Destroy();
}

