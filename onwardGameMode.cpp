// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "onwardGameMode.h"
#include "onwardCharacter.h"

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
