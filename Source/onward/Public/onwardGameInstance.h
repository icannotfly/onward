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

		if (iRemainder < 0.0)
		{
			if (TotalSeconds > 1)
			{
				Remainder = 1.0 + iRemainder;
				TotalSeconds -= 1;
			}
			else
			{
				TotalSeconds = 0;
				Remainder = 0;
			}
		}
		else
		{
			Remainder = iRemainder;
		}
	}



	//note that values can be more than their respective maximums, but cannot be negative
	FTimestamp(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond)
	{
		TotalSeconds
			= iYear * GAME_SECONDS_IN_YEAR
			+ iMonth * GAME_SECONDS_IN_MONTH
			+ iDay * GAME_SECONDS_IN_DAY
			+ iHour * GAME_SECONDS_IN_HOUR
			+ iMinute * GAME_SECONDS_IN_MINUTE
			+ iSecond;
	}



	// *
	template<typename T> FTimestamp operator* (const T &Other)
	{
		return FTimestamp(
			TotalSeconds * Other,
			Remainder * Other
		);
	}



	// /
	// note that if you pass this an int, it will truncate the quotient just like int division normally does
	template<typename T> FTimestamp operator/ (const T &Other)
	{
		if (Other == 0)
		{
			UE_LOG(HelloWorld, Warning, TEXT("FTimestamp was passed a 0 denominator; returning 0."));
			return FTimestamp(0, 0);
		}

		return FTimestamp(
			TotalSeconds / Other,
			Remainder / Other + (long double(TotalSeconds) / long double(Other) - TotalSeconds)
		);
	}



	// +
	FTimestamp operator+ (const FTimestamp &Other) const
	{
		return FTimestamp(
			TotalSeconds + Other.GetTotalSeconds(),
			Remainder + Other.GetRemainder()
		);
	}



	// -
	// if a-b and a<b, will return 0
	FTimestamp operator- (const FTimestamp &Other) const
	{
		if (*this > Other)
		{
			return FTimestamp(
				TotalSeconds - Other.GetTotalSeconds(),
				Remainder - Other.GetRemainder()
			);
		}
		else
		{
			return FTimestamp(0, 0);
		}
	}



	// <
	bool operator< (const FTimestamp &Other) const
	{
		if (TotalSeconds < Other.GetTotalSeconds() || (TotalSeconds < Other.GetTotalSeconds() && Remainder == Other.GetRemainder()))
		{
			return true;
		}
		return false;
	}



	// <=
	bool operator<= (const FTimestamp &Other) const
	{
		if (*this < Other || *this == Other)
		{
			return true;
		}
		return false;
	}



	// >
	bool operator> (const FTimestamp &Other) const
	{
		if (TotalSeconds > Other.GetTotalSeconds() || (TotalSeconds > Other.GetTotalSeconds() && Remainder == Other.GetRemainder()))
		{
			return true;
		}
		return false;
	}



	// >=
	bool operator>= (const FTimestamp &Other) const
	{
		if (*this > Other || *this == Other)
		{
			return true;
		}
		return false;
	}



	// ==
	bool operator== (const FTimestamp &Other) const
	{
		return TotalSeconds == Other.GetTotalSeconds() && Remainder == Other.GetRemainder();
	}



	// !=
	bool operator!= (const FTimestamp &Other) const
	{
		return TotalSeconds != Other.GetTotalSeconds() || Remainder != Other.GetRemainder();
	}

	

	// +=
	FTimestamp& operator+= (const FTimestamp &Other)
	{

		TotalSeconds += Other.GetTotalSeconds();
		Remainder += Other.GetRemainder();
		return *this;
	}



	// -=
	// if a-b and a<b, will return 0
	FTimestamp operator-= (const FTimestamp &Other)
	{
		if (*this > Other)
		{
			TotalSeconds -= Other.GetTotalSeconds();
			if (Remainder >= Other.GetRemainder())
			{
				Remainder -= Other.GetRemainder();
			}
			else
			{
				Remainder = 1.0 - Other.GetRemainder();
				if (TotalSeconds >= 1.0)
				{
					TotalSeconds -= 1.0;
				}
				else
				{
					TotalSeconds = 0.0;
				}
			}
		}

		return *this;
	}



	// *=
	// will not change anything if passed a negative value
	template<typename T> FTimestamp operator*= (const T &Other)
	{
		if (Other < 0.0)
		{
			UE_LOG(LogWorldTime, Warning, TEXT("FTimestamp *= called with negative value, returning original value."));
			return *this;
		}

		TotalSeconds *= Other;
		Remainder *= Other;
		CheckRemainder();
		return *this;
	}



	// /=
	// will not change anything if passed a negative or 0 value
	template<typename T> FTimestamp operator/= (const T &Other)
	{
		if (Other <= 0.0)
		{
			UE_LOG(LogWorldTime, Warning, TEXT("FTimestamp *= called with negative value, returning original value."));
			return *this;
		}

		TotalSeconds /= Other;
		Remainder /= Other;
		CheckRemainder();
		return *this;
	}



	//returns total seconds
	uint64 GetTotalSeconds() const
	{
		return TotalSeconds;
	}



	//returns current remainder
	float GetRemainder() const
	{
		return Remainder;
	}



	//returns the current year
	uint32 GetYear()
	{
		CalcDateValues();
		return Year;
	}



	//returns the current month
	uint8 GetMonth()
	{
		CalcDateValues();
		return Month;
	}



	//returns the current day
	uint8 GetDay()
	{
		CalcDateValues();
		return Day;
	}



	//returns the current hour
	uint8 GetHour()
	{
		CalcDateValues();
		return Hour;
	}



	//returns the current minute
	uint8 GetMinute()
	{
		CalcDateValues();
		return Minute;
	}



	//returns the current second
	uint8 GetSecond()
	{
		CalcDateValues();
		return Second;
	}
	


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
	


	//adds the specified YMD-HMS to this timestamp. given values must respect lower bounds but going over upper bound will cause them to flow into the next highest value. returns true if time successfully added, false if provided values were invalid or some other error; will log error.
	bool AddTime(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond = 0)
	{
		if (iYear >= 0 && iMonth >= 0 && iDay >= 0 && iHour >= 0 && iMinute >= 0 && iSecond >= 0)
		{

			TotalSeconds += iSecond;
			TotalSeconds += iMinute * GAME_SECONDS_IN_MINUTE;
			TotalSeconds += iHour * GAME_SECONDS_IN_HOUR;
			TotalSeconds += iDay * GAME_SECONDS_IN_DAY;
			TotalSeconds += iMonth * GAME_SECONDS_IN_MONTH;
			TotalSeconds += iYear * GAME_SECONDS_IN_YEAR;

			return true;
		}
		
		UE_LOG(LogWorldTime, Warning, TEXT("attempted to add one or more invalid amount(s) to a timestamp."));
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

		//make a note of when this calculation was done
		DateLastUpdated = TotalSeconds;
	}
	


	//print this timestamp's value to a human-readable FString. optional int input controlls formatting: 0 (default) = ISO 8601 (YYYY-MM-DD HH:MM:SS), 1 = ISO 8601 with debug info (YYY-MM-DD HH:MM:SS TotalSeconds,Remainder), 2 = ISO 8601 with debug and season (YYY-MM-DD HH:MM:SS TotalSeconds,Remainder Season)
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

		case 2: //ISO 8601 + TotalSeconds and Remainder + Season
			Ret = ToString(1);
			Ret += " ";
			Ret += GetSeason();
			break;


		default:
			Ret += "Invalid Formatting";
			break;
		}

		return Ret;
	}
	


	//returns an FString containting the current season
	FString GetSeason()
	{
		FString Ret = "";
		float ToY = GetTimeOfYear();

		//expand ToY range to 0.0 <= x < 4.0
		float SeasonFloat = FMath::Fmod((ToY + (1.0 / GAME_MONTHS_IN_YEAR)), 1.0) * 4.0;

		//determine prefix
		if (FMath::Fmod(SeasonFloat, 1.0) <= 0.333333)
		{
			Ret = "Early ";
		}
		else if (FMath::Fmod(SeasonFloat, 1.0) > -0.666667)
		{
			Ret = "Late ";
		}
		else
		{
			Ret = "Mid ";
		}

		//determine prefix
		switch (FMath::FloorToInt(SeasonFloat))
		{
		case 0: Ret += "Winter"; break;
		case 1: Ret += "Spring"; break;
		case 2: Ret += "Summer"; break;
		case 3: Ret += "Autumn"; break; //it was gonna be "fall" but then they'd be different widths
		default: Ret = "! ERROR !"; break;
		}

		return Ret;
	}



	//returns a float in the range 0.0 (beginning of year) to 1.0 (end of year)
	float GetTimeOfYear() { return  FMath::Fmod(float(TotalSeconds), GAME_SECONDS_IN_YEAR) / GAME_SECONDS_IN_YEAR; }



	//returns a float in the range 0.0 (beginning of day, 00:00) to 1.o (end of day, 24:00)
	float GetTimeOfDay() { return FMath::Fmod(float(TotalSeconds), GAME_SECONDS_IN_DAY) / GAME_SECONDS_IN_DAY; }
};



UCLASS()
class ONWARD_API UonwardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
	
private:
	
	UPROPERTY(Replicated) FTimestamp WorldTime;


public:

	FTimestamp* GetWorldTime() { return &WorldTime; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
