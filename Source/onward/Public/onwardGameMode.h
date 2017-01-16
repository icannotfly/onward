// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "onwardGameMode.generated.h"

UCLASS(minimalapi)
class AonwardGameMode : public AGameMode
{
	GENERATED_BODY()



public:

	AonwardGameMode();

	virtual void Tick(float DeltaSeconds) override;



	//shorthand way of setting world time
	UFUNCTION(exec) void Time(FString iTime);

	//sets the rate of time's passage. units passed are real-world minutes per in-game day. valid range is (0, 1440]
	UFUNCTION(exec) void TimeRate(float iNewRate);

	//set the world time to a specific point in time. not all values need to be supplied, but incoming values will be assumed in descending order starting with year. values do not need to obey upper limits.
	UFUNCTION(exec) void TimeSet(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond);

	//adds the specified amount to the current world time. values do not need to obey upper limits.
	UFUNCTION(exec) void TimeAdd(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond);

	//forward time to the specified point
	UFUNCTION() void ForwardTimeTo(FTimestamp iTargetTime);

	//toggle visiblity of the crosshair
	UFUNCTION(exec) void ToggleCrosshair();

	//toggle visibilty of the debug bars
	UFUNCTION(exec) void ToggleDebugBars();

	//do some tests; exact nature depends on the tests contained in this function at the time of compilation
	UFUNCTION(exec) void DoTests();
};



