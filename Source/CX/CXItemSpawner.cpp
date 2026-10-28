#include "CXItemSpawner.h"
#include "CXPickupActor.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACXItemSpawner::ACXItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ACXItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnItem();
	}
}

void ACXItemSpawner::SpawnItem()
{
	if (HasAuthority() == false || ItemClass == nullptr)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnedItem = GetWorld()->SpawnActor<ACXPickupActor>(ItemClass, GetActorTransform(), Params);
	
	if (IsValid(SpawnedItem))
	{
		SpawnedItem->OnDestroyed.AddDynamic(this, &ThisClass::OnItemDestroyed);
	}
}

void ACXItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	SpawnedItem = nullptr;

	if (IsValid(GetWorld()))
	{
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &ThisClass::SpawnItem, RespawnDelay, false);
	}
}

