// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardWorldSettings.h"



AonwardWorldSettings::AonwardWorldSettings()
{
	UE_LOG(HelloWorld, Log, TEXT("WorldSettings spawned."));

	//need to calc this at least once right off the bat, otherwise it stays at 0
	CalcRealWorldToInGameTimeConversionFactor();
}



//returns true if iTestValue is within RealWorldMinutesPerInGameDayAcceptableRange (inclusive), false if outside range
bool AonwardWorldSettings::ValidateRealWorldMinutesPerIngameDayValue(float iTestValue)
{
	return
		iTestValue <= RealWorldMinutesPerInGameDayAcceptableRange.X &&
		iTestValue >= RealWorldMinutesPerInGameDayAcceptableRange.Y;
}



//returns true if value was successfully changed, false if not. will print error to log.
bool AonwardWorldSettings::SetRealWorldMinutesPerInGameDay(float iNewValue)
{
	FString MsgRoot = GetName() + FString("::SetRealWorldMinutesPerInGameDay:");

	if (iNewValue == RealWorldMinutesPerInGameDay)
	{
		UE_LOG(LogWorldTime, Warning, TEXT("%s will not change value: same as pervious value."), *(MsgRoot));
		return false;
	}
	else if (ValidateRealWorldMinutesPerIngameDayValue(iNewValue))
	{
		RealWorldMinutesPerInGameDay = iNewValue;
		UE_LOG(LogWorldTime, Log, TEXT("%s value changed. Time now passes at the rate of %d real-world minutes per in-game day."), *(MsgRoot), RealWorldMinutesPerInGameDay);
		CalcRealWorldToInGameTimeConversionFactor();
		return true;
	}
	else
	{
		UE_LOG(LogWorldTime, Log, TEXT("%s cannot change value: outside acceptable range (%d,%d)."), *(MsgRoot), RealWorldMinutesPerInGameDayAcceptableRange.X, RealWorldMinutesPerInGameDayAcceptableRange.Y);
		return true;
	}
}



void AonwardWorldSettings::CalcRealWorldToInGameTimeConversionFactor()
{
	RealWorldToInGameTimeConversionFactor = (GAME_HOURS_PER_DAY * GAME_MINUTES_PER_HOUR * GAME_SECONDS_PER_MINUTE) / (GetRealWorldMinutesPerInGameDay() * 60.0);
			
	UE_LOG(LogWorldTime, Log, TEXT("RW to IG time conversion factor is now %d"), RealWorldToInGameTimeConversionFactor);
}