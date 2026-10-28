#include "CXSpeedBuffActor.h"
#include "CXPlayerState.h"

void ACXSpeedBuffActor::ApplyEffect(ACXPlayerState* PlayerState)
{
	PlayerState->AddBonusMaxSpeed(BuffAmount);
}

