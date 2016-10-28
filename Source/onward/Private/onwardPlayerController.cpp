// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardPlayerCameraManager.h"
#include "onwardPlayerController.h"



AonwardPlayerController::AonwardPlayerController(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = AonwardPlayerCameraManager::StaticClass();
}
