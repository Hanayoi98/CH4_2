#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXItemSpawner.generated.h"

class ACXPickupActor;

UCLASS()
class CX_API ACXItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACXItemSpawner();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnItem();

	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<ACXPickupActor> ItemClass;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float RespawnDelay = 2.0f;

	UPROPERTY()
	TObjectPtr<ACXPickupActor> SpawnedItem;

	FTimerHandle RespawnTimer;


};
