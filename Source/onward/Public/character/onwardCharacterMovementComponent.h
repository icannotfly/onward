// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "onwardCharacterMovementComponent.generated.h"

UCLASS()
class ONWARD_API UonwardCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()



		virtual float GetMaxSpeed() const override;
};
