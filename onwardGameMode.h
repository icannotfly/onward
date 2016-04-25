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



	//sets the rate of time's passage. units passed are real-world minutes per in-game day. valid range is (0, 1440]
	UFUNCTION(exec) void TimeRate(float iNewRate);
};



