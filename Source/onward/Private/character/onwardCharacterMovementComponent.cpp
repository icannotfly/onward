// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "character/onwardCharacter.h"
#include "character/onwardCharacterMovementComponent.h"



float UonwardCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	const AonwardCharacter* CharOwner = Cast<AonwardCharacter>(PawnOwner);
	if (CharOwner)
	{
		if (CharOwner->IsSprinting())
		{
			MaxSpeed *= CharOwner->GetSprintingSpeedModifier();
		}
		else if (CharOwner->IsWalking())
		{
			MaxSpeed *= CharOwner->GetWalkingSpeedModifier();
		}

		//TODO crouching speed modifier goes here
		/*else if (CharacterOwner->IsCrouching())
		{
			MaxSpeed *= CharOwner->GetCrouchingSpeedModifier();
		}*/
		//TODO prone too, if that'll be a thing
		/*else if (CharacterOwner->IsProne())
		{
			MaxSpeed *= CharOwner->GetProneSpeedModifier();
		}*/
	}

	return MaxSpeed;
}
