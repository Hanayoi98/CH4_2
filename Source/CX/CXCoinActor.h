#pragma once

#include "CoreMinimal.h"
#include "CXPickupActor.h"
#include "GameFramework/Actor.h"
#include "CXCoinActor.generated.h"

UCLASS()
class CX_API ACXCoinActor : public ACXPickupActor
{
	GENERATED_BODY()

protected:
	virtual void ApplyEffect(ACXPlayerState* PlayerState) override;

private:	
	UPROPERTY(EditDefaultsOnly, Category = "Coin")
	int32 CoinAmount = 10;
};
