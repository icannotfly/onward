#pragma once
//Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "onwardHUD.h"
#include "onwardGameInstance.h"
#include "SlateBasics.h"

class SDebugBarsWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SDebugBarsWidget)
	{

	}

	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class AonwardHUD>, OwnerHUD)

	SLATE_END_ARGS()

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Needed for every widget
	/////Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

private:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Pointer to our parent HUD
	/////To make sure HUD's lifetime is controlled elsewhere, use "weak" ptr.
	/////HUD has "strong" pointer to Widget,
	/////circular ownership would prevent/break self-destruction of hud/widget (cause memory leak).
	TWeakObjectPtr<class AonwardHUD> OwnerHUD;

	

	//time of year string
	TAttribute<FText> WorldTime; 	//stores the binding for world time debug bar display
	FText GetWorldTime() const; 	//gets and formats world time string

	//fractional time of day
	TAttribute<float> WorldTimeOfDay;
	TOptional<float> GetWorldTimeOfDay() const;

	//stores season
	TAttribute<FText> WorldSeason;
	FText GetWorldSeason() const;

	//fractional time of year
	TAttribute<float> WorldTimeOfYear;
	TOptional<float> GetWorldTimeOfYear() const;



	/**
	* A reference to the Slate Style used for this HUD's widgets.
	**/
	const struct FGlobalStyle* HUDStyle;
};
