#include "CXPlayerState.h"
#include "Net/UnrealNetwork.h"

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACXPlayerState::AddCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0)
	{
		return;
	}

	Coins += Amount;
	OnRep_Coins();
}

bool ACXPlayerState::SpendCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0 || Coins < Amount)
	{
		return false;
	}

	Coins -= Amount;
	OnRep_Coins();
	return true;
}

void ACXPlayerState::AddBonusMaxSpeed(float Amount)
{
	if (HasAuthority() == false || Amount <= 0.0f)
	{
		return;
	}

	BonusMaxSpeed += Amount;
	OnRep_BonusMaxSpeed();
}

void ACXPlayerState::OnRep_Coins()
{
}

void ACXPlayerState::OnRep_BonusMaxSpeed()
{
}

