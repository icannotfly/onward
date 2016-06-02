// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "onwardCharacter.h"
#include "onwardGameInstance.h"
#include "onwardWorldSettings.h"
#include "onwardHUD.h"
#include "onwardGameMode.h"

AonwardGameMode::AonwardGameMode()
{
	//hello world
	UE_LOG(HelloWorld, Log, TEXT("GameMode online; hello, world."))



	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}



	//Set the hud class to use our custom HUD by default.
	this->HUDClass = AonwardHUD::StaticClass();
}



void AonwardGameMode::Tick(float DeltaSeconds)
{
	//increment world time once per frame
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());
	if (GI)
	{
		AonwardWorldSettings* WS = Cast<AonwardWorldSettings>(GetWorldSettings());
		if (WS)
		{
			float SecondsToAdd = DeltaSeconds * WS->GetRealWorldToInGameTimeConversionFactor();
			GI->GetWorldTime()->AddSeconds(SecondsToAdd);

			UE_LOG(LogWorldTime, Verbose, TEXT("time: %s  -  added %f sec to world time"), *(GI->GetWorldTime()->ToString(2)), SecondsToAdd);
		}
	}



	Super::Tick(DeltaSeconds);
}



void AonwardGameMode::Time(FString iTime)
{
	FString t = iTime.ToLower();
	uint8 TargetHour = 0;

	//set general time of day
	/*0600*/ if		 (t == "morning")	{ TargetHour = 6; }
	/*1000*/ else if (t == "day")		{ TargetHour = 10; }
	/*1200*/ else if (t == "midday")	{ TargetHour = 12; }
	/*1400*/ else if (t == "afternoon")	{ TargetHour = 14; }
	/*1800*/ else if (t == "evening")	{ TargetHour = 18; }
	/*2200*/ else if (t == "night")		{ TargetHour = 22; }
	/*2400*/ else if (t == "midnight")	{ TargetHour = 0; }

	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("%s::Time(): I don't understand what you mean by %s."), *GetName(), *iTime);
		return;
	}
}



void AonwardGameMode::TimeRate(float iNewRate)
{
	AonwardWorldSettings* WS = Cast<AonwardWorldSettings>(GetWorldSettings());

	if (WS)
	{
		WS->SetRealWorldMinutesPerInGameDay(iNewRate); //validation is done in here
	}
	
	UE_LOG(LogWorldTime, Warning, TEXT("Could not change time rate: could not find WorldSettings."));
}



void AonwardGameMode::TimeSet(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		FTimestamp TargetTime = FTimestamp(iYear, iMonth, iDay, iHour, iMinute, iSecond);

		if (TargetTime.GetTotalSeconds() > GI->GetWorldTime()->GetTotalSeconds()) //this is all the validation we need
		{
			ForwardTimeTo(TargetTime);

			UE_LOG(LogWorldTime, Log, TEXT("changed world time to %s"), *(GI->GetWorldTime()->ToString(1)));
		}
		else
		{
			UE_LOG(LogWorldTime, Warning, TEXT("Could not set time: could not find GameInstance."));
		}
	}
}



void AonwardGameMode::TimeAdd(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		ForwardTimeTo(FTimestamp(GI->GetWorldTime()->GetTotalSeconds() + FTimestamp(iYear, iMonth, iDay, iHour, iMinute, iSecond).GetTotalSeconds(), GI->GetWorldTime()->GetRemainder()));
	}
	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: could not find GameInstance."));
		return;
	}
}



void AonwardGameMode::ForwardTimeTo(FTimestamp iTargetTime)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		FTimestamp* WorldTime = GI->GetWorldTime();

		//make sure this is indeed in the future
		if (iTargetTime.GetTotalSeconds() > WorldTime->GetTotalSeconds())
		{
			UE_LOG(LogWorldTime, Log, TEXT("Forwarding time to %s"), *(iTargetTime.ToString()));

			while (iTargetTime.GetTotalSeconds() > WorldTime->GetTotalSeconds())
			{
				//add random amount between 1 month and 1 year
				float SecondsToAdd = FMath::FRandRange(GAME_SECONDS_IN_MONTH, GAME_SECONDS_IN_YEAR);

				//overshoot detection
				if (WorldTime->GetTotalSeconds() + SecondsToAdd > iTargetTime.GetTotalSeconds())
				{
					//we overshot; clamp
					WorldTime->AddSeconds(iTargetTime.GetTotalSeconds() - WorldTime->GetTotalSeconds());
				}
				else
				{
					WorldTime->AddSeconds(SecondsToAdd);
				}

				UE_LOG(LogWorldTime, Log, TEXT("    added %f seconds, time is now %s"), SecondsToAdd, *(WorldTime->ToString()));
			}
		}
		else
		{
			UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: cannot go backwards in time."));
			return;
		}
	}
	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: could not find GameInstance."));
		return;
	}
}
