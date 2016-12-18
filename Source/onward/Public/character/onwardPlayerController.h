// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "onwardPlayerController.generated.h"



UCLASS()
class ONWARD_API AonwardPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AonwardPlayerController(const FObjectInitializer& ObjectInitializer);
};
