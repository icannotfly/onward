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
};



