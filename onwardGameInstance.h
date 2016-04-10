// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "onwardGameInstance.generated.h"



USTRUCT()
struct FTimestamp
{
	GENERATED_USTRUCT_BODY()



private:

	//number of full in-game seconds that have elapsed since 0-0-0 0:00:00; analagous to unix time
	UPROPERTY() uint64 TotalSeconds = 0;

	//sub-1.0 second amount that cannot be expressed in TotalSeconds
	UPROPERTY() float Remainder = 0.0;



	//date values
	UPROPERTY() uint32 Year = 0;
	UPROPERTY() uint8 Month = 0;
	UPROPERTY() uint8 Day = 0;
	UPROPERTY() uint8 Hour = 0;
	UPROPERTY() uint8 Minute = 0;
	UPROPERTY() uint8 Second = 0;



	//TotalSeconds value when Date values were last updated. for static timestamps, we do not need to update Date values every time they're asked for.
	UPROPERTY() uint64 DateLastUpdated = 0;



public:

	FTimestamp(){};

	FTimestamp(uint64 iTotalSeconds, float iRemainder)
	{
		TotalSeconds = iTotalSeconds;
		Remainder = iRemainder;
	}



	const uint64 GetTotalSeconds() { return TotalSeconds; }
	const float GetRemainder() { return Remainder; }



	//if we have more than a full second in the remainder, add it to the TotalSeconds count
	void CheckRemainder()
	{
		if (Remainder >= 1.0)
		{
			TotalSeconds += FMath::FloorToInt(Remainder);
			Remainder -= FMath::FloorToFloat(Remainder);
		}
	}



	//add a valid amount of seconds to this timestamp; returns true if successful, false if value passed was negative or otherwise invalid
	bool AddSeconds(float iSeconds)
	{
		if (iSeconds > 0.0)
		{
			TotalSeconds += FMath::FloorToInt(iSeconds);
			Remainder += iSeconds - FMath::FloorToFloat(iSeconds);
			CheckRemainder();
			return true;
		}
		else
		{
			UE_LOG(LogWorldTime, Warning, TEXT("attempted to add an invalid amount of seconds to a timestamp."));
		}

		return false;
	}



	//calculate date values; will skip calculation if values are current and do not need to be updated.
	void CalcDateValues()
	{
		//skip if values are current
		if (DateLastUpdated == TotalSeconds)
		{
			return;
		}

		//otherwise, do calc
		uint64 RollingTotal = TotalSeconds;

		Year = FMath::FloorToInt(RollingTotal / GAME_SECONDS_IN_YEAR);
		RollingTotal -= Year * GAME_SECONDS_IN_YEAR;

		Month = FMath::FloorToInt(RollingTotal / GAME_SECONDS_IN_MONTH);
		RollingTotal -= Month * GAME_SECONDS_IN_MONTH;

		Day = FMath::FloorToInt(RollingTotal / GAME_SECONDS_IN_DAY);
		RollingTotal -= Day * GAME_SECONDS_IN_DAY;

		Hour = FMath::FloorToInt(RollingTotal / GAME_SECONDS_IN_HOUR);
		RollingTotal -= Hour * GAME_SECONDS_IN_HOUR;

		Minute = FMath::FloorToInt(RollingTotal / GAME_SECONDS_IN_MINUTE);
		RollingTotal -= Minute * GAME_SECONDS_IN_MINUTE;

		Second = RollingTotal;
	}



	//print this timestamp's value to a human-readable FString. optional int input controlls formatting: 0 (default) = ISO 8601 (YYYY-MM-DD HH:MM:SS), 1 = ISO 8601 with debug info (YYY-MM-DD HH:MM:SS TotalSeconds,Remainder)
	FString ToString(uint8 iFormatting = 0)
	{
		FString Ret = "";;

		CalcDateValues();

		switch (iFormatting)
		{
		case 0: //ISO 8601 (-ish)
			if (Year < 10) { Ret += "00"; }
			else if (Year < 100) { Ret += "0"; }
			Ret.AppendInt(Year);
			Ret += "-";

			if (Month < 10) { Ret += "0"; }
			Ret.AppendInt(Month);
			Ret += "-";

			if (Day < 10) { Ret += "0"; }
			Ret.AppendInt(Day);
			Ret += " ";

			if (Hour < 10) { Ret += "0"; }
			Ret.AppendInt(Hour);
			Ret += ":";

			if (Minute < 10) { Ret += "0"; }
			Ret.AppendInt(Minute);

			Ret += ":";

			if (Second < 10) { Ret += "0"; }
			Ret.AppendInt(Second);
			break;

		case 1: //ISO 8601 + TotalSeconds and Remainder
			Ret = ToString(0);
			Ret += " (";
			Ret.AppendInt(TotalSeconds);
			Ret += ", ";
			Ret += FString::SanitizeFloat(Remainder).Left(5);
			Ret += ")";
			break;

		default:
			Ret = "Invalid Formatting";
			break;
		}

		return Ret;
	}
};



UCLASS()
class ONWARD_API UonwardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
	
private:
	
	FTimestamp WorldTime;


public:

	FTimestamp* GetWorldTime() { return &WorldTime; }

};
