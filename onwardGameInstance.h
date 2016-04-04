// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "onwardGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ONWARD_API UonwardGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
	
private:
	
	float WorldTime = 0.f; //TODO turn into timestamp



public:

	float* GetWorldTime() { return &WorldTime; }

};
