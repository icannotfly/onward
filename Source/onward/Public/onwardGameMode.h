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

	//add the specified amount of health to the player
	UFUNCTION(exec) void AddHealth(float HealthAmount);

	//remove the specified amout of heath from the player - can kill
	UFUNCTION(exec) void RemoveHealth(float HealthAmount);

	//add the specified amount of stamina to the player
	UFUNCTION(exec) void AddStamina(float StaminaAmount);

	//remove the specified amout of stamina from the player
	UFUNCTION(exec) void RemoveStamina(float StaminaAmount);

	//add the specified amount of mana to the player
	UFUNCTION(exec) void AddMana(float ManaAmount);

	//remove the specified amout of mana from the player
	UFUNCTION(exec) void RemoveMana(float ManaAmount);

	//toggle visiblity of the crosshair
	UFUNCTION(exec) void ToggleCrosshair();

	//do some tests; exact nature depends on the tests contained in this function at the time of compilation
	UFUNCTION(exec) void DoTests();
};



