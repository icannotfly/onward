// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "onwardPlayerController.generated.h"



UCLASS()
class ONWARD_API AonwardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AonwardPlayerController(const FObjectInitializer& ObjectInitializer);



public:

	//add the specified amount of stamina to this character
	UFUNCTION(exec) void AddStamina(float iAmountToAdd);

	//remove the specified amount of stamina from this player. pass "true" as second value to allow passing out from overexertion, will not pass out otherwise
	UFUNCTION(exec) void RemoveStamina(float iAmountToAdd, FString ShouldPassOut = "no way jose");
};
