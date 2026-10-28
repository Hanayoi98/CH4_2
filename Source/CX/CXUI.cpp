// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UCXUI::UpdateCoins(int32 NewCoins)
{
	OnCoinsUpdate(NewCoins);
}

void UCXUI::UpdateTime(int32 NewTime)
{
	OnTimeUpdate(NewTime);
}

void UCXUI::UpdateStatus(const FText& NewStatus)
{
	OnStatusUpdate(NewStatus);
}

void UCXUI::ShowResult(const FText& NewResult)
{
	OnResultUpdate(NewResult);
}

void UCXUI::UpdateBonusSpeed(float NewBonusSpeed)
{
	OnBonusSpeedUpdate(NewBonusSpeed);
}