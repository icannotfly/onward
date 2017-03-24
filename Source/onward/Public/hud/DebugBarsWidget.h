#pragma once
//Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "onwardHUD.h"
#include "onwardGameInstance.h"
#include "SlateBasics.h"

class SDebugBarsWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SDebugBarsWidget) { }

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



	// reference to the Slate Style used for this HUD's widgets.
	const struct FGlobalStyle* DebugStyle;



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



	//player health string
	TAttribute<FText> PlayerHealthString;
	FText GetPlayerHealthString() const;

	//player health percentage
	TAttribute<float> PlayerHealthPercentage;
	TOptional<float> GetPlayerHealthPercentage() const;



	//player stamina string
	TAttribute<FText> PlayerStaminaString;
	FText GetPlayerStaminaString() const;

	//player stamina percentage
	TAttribute<float> PlayerStaminaPercentage;
	TOptional<float> GetPlayerStaminaPercentage() const;



	//player mana string
	TAttribute<FText> PlayerManaString;
	FText GetPlayerManaString() const;

	//player mana percentage
	TAttribute<float> PlayerManaPercentage;
	TOptional<float> GetPlayerManaPercentage() const;

	//mana bar color
	FSlateColorBrush ManaBarBrush = FSlateColorBrush(FLinearColor::Blue);



	//player movement status (running, jumping, whatever)
	TAttribute<FText> PlayerMovementStatusString;
	FText GetPlayerMovementStatusString() const;

	//player movement velocity
	TAttribute<float> PlayerMovementVelocity;
	TOptional<float> GetPlayerMovementVelocityXY() const; //note that this only returns XY velocity, we don't care about jumping
	TOptional<float> GetPlayerMovementVelocityXYPercentage() const;
};
