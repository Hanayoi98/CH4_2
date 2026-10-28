#include "CXGameState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

bool ACXGameState::IsWinner(const APlayerState* PlayerState) const
{
	for (const TObjectPtr<APlayerState>& Winner : Winners)
	{
		if (Winner == PlayerState)
		{
			return true;
		}
	}

	return false;
}

void ACXGameState::SetCurrentState(ECXGameState NewState)
{
	if (HasAuthority() == false)
	{
		return;
	}

	CurrentState = NewState;
}

void ACXGameState::SetRemainingTime(int32 NewTime)
{
	if (HasAuthority() == false)
	{
		return;
	}

	RemainingTime = NewTime;
}

void ACXGameState::SetWinners(const TArray<APlayerState*>& NewWinners)
{
	if (HasAuthority() == false)
	{
		return;
	}

	Winners.Reset();
	for (APlayerState* Winner : NewWinners)
	{
		Winners.Add(Winner);
	}
}

void ACXGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentState);
	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, Winners);
}
