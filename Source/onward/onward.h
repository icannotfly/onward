// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __ONWARD_H__
#define __ONWARD_H__

//#include "EngineMinimal.h"
#include "Engine.h" //needed for GEngine calls
#include "Net/UnrealNetwork.h" //needed for teabagging friends



#define GAME_MONTHS_IN_YEAR 8.0
#define GAME_DAYS_IN_MONTH 20.0
#define GAME_HOURS_IN_DAY 24.0
#define GAME_MINUTES_IN_HOUR 60.0
#define GAME_SECONDS_IN_MINUTE 60.0

//these values MUST be recalculated if any of the above values change
#define GAME_SECONDS_IN_YEAR 13824000.0
#define GAME_SECONDS_IN_MONTH 1728000.0
#define GAME_SECONDS_IN_DAY 86400.0
#define GAME_SECONDS_IN_HOUR 3600.0



//thanks, rama!
//use like:
//    UE_LOG(HelloWorld, Log, TEXT("%s::%s() at line (%s): error message"), *(CURR_CLASS), *(CURR_FUNCTION), *(CURR_LINE));

//current class - looks like    AonwardGameMode
#define CURR_CLASS FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":")))

//current function - looks like    DoTests
#define CURR_FUNCTION FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 )

//current line number - looks like    437
#define CURR_LINE FString::FromInt(__LINE__)

//signature of current function - looks like    void __cdecl AonwardGameMode::DoTests(void)
#define CURR_FUNC_SIG (FString(__FUNCSIG__))

//everything mushed into one - looks like    void __cdecl AonwardGameMode::DoTests(void) @ 439
#define CURR_FUNC_CALL (CURR_FUNC_SIG + " @ " + CURR_LINE)



DECLARE_LOG_CATEGORY_EXTERN(HelloWorld, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWorldTime, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCombat, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCharacterMovement, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCharacterVitals, Log, All);

#endif
