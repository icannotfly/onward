// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "onwardPlayerCameraManager.generated.h"



UCLASS()
class ONWARD_API AonwardPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	//need to add constructor as per https://answers.unrealengine.com/questions/142757/simple-actor-constructor.html idk why though
	AonwardPlayerCameraManager(const FObjectInitializer& ObjectInitializer);
	
	
	
public:

	//returns the default FoV
	float GetDefaultFoV() const;

	//returns the ADS FoV
	float GetADSFoV() const;



private:

	//default fov
	float DefaultFoV;

	//for... bows? squinting? idk
	float ADSFoV;

	//update the fov
	virtual void UpdateCamera(float DeltaTime) override;
};
