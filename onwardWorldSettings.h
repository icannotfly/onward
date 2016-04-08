// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/WorldSettings.h"
#include "onwardWorldSettings.generated.h"



UCLASS()
class ONWARD_API AonwardWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
	
	
private:

	FVector2D RealWorldMinutesPerInGameDayAcceptableRange = FVector2D(0.000001, 1440.0);
	UPROPERTY(EditAnywhere, Category = "Time", meta = (UIMin = "0.000001", UIMax = "1440.0")) float RealWorldMinutesPerInGameDay = 24.0; 

	bool ValidateRealWorldMinutesPerIngameDayValue(float iTestValue);

	//multiply a real-world seconds value by this number to get the corresponding amount of in-game seconds. this value is updated every time the length of an in-game day changes.
	float RealWorldToInGameTimeConversionFactor = 0.0;



public:

	AonwardWorldSettings();

	const float GetRealWorldMinutesPerInGameDay() { return RealWorldMinutesPerInGameDay; }
	bool SetRealWorldMinutesPerInGameDay(float iNewValue);

	const float GetRealWorldToInGameTimeConversionFactor() { return RealWorldToInGameTimeConversionFactor; }
	void CalcRealWorldToInGameTimeConversionFactor();
};
