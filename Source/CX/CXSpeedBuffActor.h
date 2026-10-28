#pragma once

#include "CoreMinimal.h"
#include "CXPickupActor.h"
#include "CXSpeedBuffActor.generated.h"

UCLASS()
class CX_API ACXSpeedBuffActor : public ACXPickupActor
{
	GENERATED_BODY()
	
protected:
	virtual void ApplyEffect(ACXPlayerState* PlayerState) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Buff")
	float BuffAmount = 10.0f;
};
