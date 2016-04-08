// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "onwardGameInstance.generated.h"



USTRUCT()
struct FTimestamp
{
	GENERATED_USTRUCT_BODY()



private:

	UPROPERTY() uint64 TotalSeconds = 0;

	UPROPERTY() float Remainder = 0.0;



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
