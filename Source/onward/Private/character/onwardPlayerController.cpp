// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardPlayerCameraManager.h"
#include "onwardCharacter.h"
#include "onwardPlayerController.h"



AonwardPlayerController::AonwardPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AonwardPlayerCameraManager::StaticClass();
}



void AonwardPlayerController::AddStamina(float iAmountToAdd)
{
	if (Cast<AonwardCharacter>(GetPawn()))
	{
		Cast<AonwardCharacter>(GetPawn())->ChangeStamina(iAmountToAdd);
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("%s failed to get reference to our pawn"), *(CURR_FUNC_CALL));
	}
}

//remove the specified amount of stamina from this player. pass "true" as second value to allow passing out from overexertion, will not pass out otherwise
void AonwardPlayerController::RemoveStamina(float iAmountToAdd, FString ShouldPassOut)
{
	if (Cast<AonwardCharacter>(GetPawn()))
	{
		Cast<AonwardCharacter>(GetPawn())->ChangeStamina((iAmountToAdd * -1.0), (ShouldPassOut.ToLower() == "true"));
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("%s failed to get reference to our pawn"), *(CURR_FUNC_CALL));
	}
}