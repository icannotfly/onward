// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "onwardCharacterMovementComponent.generated.h"

UENUM(BlueprintType)
namespace EonwardMovementMode
{
	enum Type
	{
		/*EMM_Prone			UMETA(DisplayName = "Prone"),*/ //unimplemented
		EMove_Walking		UMETA(DisplayName = "Walking"),
		EMove_Running		UMETA(DisplayName = "Running"),
		EMove_Sprinting		UMETA(DisplayName = "Sprinting"),

		EMove_MAX			UMETA(Hidden),
	};
}

UENUM(BlueprintType)
namespace EonwardStance
{
	enum Type
	{
		EStance_Standing	UMETA(DisplayName = "Standing"),
		EStance_Crouching	UMETA(DisplayName = "Crouching"),
		EStance_Prone		UMETA(DisplayName = "Prone"),

		EStance_MAX			UMETA(Hidden),
	};
}

UCLASS()
class ONWARD_API UonwardCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()


		
	virtual float GetMaxSpeed() const override;

public:

	//returns a human-readable string representing our current movement mode
	FString GetMovementModeString();



protected:

	//our current movement mode; this is basically the speed at which we're moving
	int8 CurrentMovementMode;
};
