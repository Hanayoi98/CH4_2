#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXPickupActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;
class ACXPlayerState;

UCLASS()
class CX_API ACXPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACXPickupActor();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void ApplyEffect(ACXPlayerState* PlayerState) {}

	UFUNCTION()
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TObjectPtr<UParticleSystem> PickupParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	TObjectPtr<USoundBase> PickupSound;

private:
	bool bConsumed = false;

};
