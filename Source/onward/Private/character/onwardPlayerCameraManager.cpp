// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardCharacter.h"
#include "onwardPlayerCameraManager.h"



AonwardPlayerCameraManager::AonwardPlayerCameraManager(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = 90.0f;
	ADSFoV = 65.0f;

	ViewPitchMin = -84.0f;
	ViewPitchMax = 88.0f;
	bAlwaysApplyModifiers = true;
}


void AonwardPlayerCameraManager::UpdateCamera(float DeltaTime)
{
	/*AonwardCharacter* MyPawn = PCOwner ? Cast<AonwardCharacter>(PCOwner->GetPawn()) : NULL;
	if (MyPawn)
	{
		const float TargetFOV = MyPawn->IsTargeting() ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo(DefaultFOV, TargetFOV, DeltaTime, 20.0f);
		SetFOV(DefaultFOV);
	}*/

	Super::UpdateCamera(DeltaTime);
}



float AonwardPlayerCameraManager::GetDefaultFoV() const
{
	return DefaultFoV;
}


float AonwardPlayerCameraManager::GetADSFoV() const
{
	return ADSFoV;
}
