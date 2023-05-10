

#include "GoldCollector.h"
#include "Gold.h"

UGoldCollector::UGoldCollector():
	m_offset(FVector(0, 0, 0.1f)),
	m_searchTimer(0.f)
{
	PrimaryComponentTick.bCanEverTick = true;

	SweepSphere = FCollisionShape::MakeSphere(Radius);
	TraceParams = FCollisionQueryParams();
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(GetOwner());
}


void UGoldCollector::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGoldCollector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Search for near-by gold every SEARCH_TIME
	if (m_searchTimer >= SEARCH_TIME)
	{
		//Cast sphere and check if gold is around
		bool hit = SearchForNearGold();

		if (hit)
		{
			//if at least one gold was hit, move towards the player
			MoveAllGoldToPlayer();
		}

		m_searchTimer = 0.f;
	}

	m_searchTimer += DeltaTime;
}

void UGoldCollector::MoveAllGoldToPlayer()
{
	for (const FHitResult hitObj : OutHits)
	{
		AGold* Gold = Cast<AGold>(hitObj.GetActor());
		if (Gold)
		{
			//per gold, move it towards the player
			Gold->AttractTowardsPlayer(GetOwner());
		}
	}
}

//The actual casting of sphere to search for gold
//The ECC_GameTraceChannel2 only casts a channel "Collectables" and for now, only Gold is responding to it
bool UGoldCollector::SearchForNearGold()
{
	bool hit = GetWorld()->SweepMultiByChannel
	(
		OutHits,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + m_offset,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		SweepSphere,
		TraceParams
	);



	return hit;
}

