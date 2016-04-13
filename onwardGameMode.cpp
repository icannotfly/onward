// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "onwardCharacter.h"
#include "onwardGameInstance.h"
#include "onwardWorldSettings.h"
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

			UE_LOG(LogWorldTime, Log, TEXT("time: %s  -  added %f sec to world time"), *(GI->GetWorldTime()->ToString(2)), SecondsToAdd);
		}
	}



	Super::Tick(DeltaSeconds);
}
