#include "CXCoinActor.h"
#include "CXPlayerState.h"

void ACXCoinActor::ApplyEffect(ACXPlayerState* PlayerState)
{
	PlayerState->AddCoins(CoinAmount);
}

