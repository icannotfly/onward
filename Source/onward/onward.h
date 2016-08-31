// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __ONWARD_H__
#define __ONWARD_H__

//#include "EngineMinimal.h"
#include "Engine.h" //needed for GEngine calls



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
// UE_LOG(HelloWorld, Log, TEXT("%s::%s() (%s)"), *(CURR_CLASS), *(CURR_FUNCTION), *(CURR_LINE));
#define CURR_CLASS FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) //will print current class
#define CURR_FUNCTION FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ) //will print current function
#define CURR_LINE FString::FromInt(__LINE__)



DECLARE_LOG_CATEGORY_EXTERN(HelloWorld, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWorldTime, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogCombat, Log, All);

#endif
