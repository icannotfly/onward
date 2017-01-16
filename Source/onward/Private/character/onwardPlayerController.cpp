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

void AonwardPlayerController::RemoveStamina(float iAmountToRemove, FString ShouldPassOut)
{
	if (Cast<AonwardCharacter>(GetPawn()))
	{
		Cast<AonwardCharacter>(GetPawn())->ChangeStamina((iAmountToRemove * -1.0), (ShouldPassOut.ToLower() == "true"));
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("%s failed to get reference to our pawn"), *(CURR_FUNC_CALL));
	}
}



void AonwardPlayerController::AddMana(float iAmountToAdd)
{
	if (Cast<AonwardCharacter>(GetPawn()))
	{
		Cast<AonwardCharacter>(GetPawn())->ChangeMana(iAmountToAdd);
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("%s failed to get reference to our pawn"), *(CURR_FUNC_CALL));
	}
}

void AonwardPlayerController::RemoveMana(float iAmountToRemove)
{
	if (Cast<AonwardCharacter>(GetPawn()))
	{
		Cast<AonwardCharacter>(GetPawn())->ChangeMana((iAmountToRemove * -1.0));
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("%s failed to get reference to our pawn"), *(CURR_FUNC_CALL));
	}
}
