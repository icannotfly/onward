#pragma once
//Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "onwardHUD.h"
#include "SlateBasics.h"

class SWatermarkWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SWatermarkWidget)
		: _ItemImage(nullptr)
	{ }

	// See private declaration of OwnerHUD below.
	SLATE_ARGUMENT(TWeakObjectPtr<class AonwardHUD>, OwnerHUD)

	//the watermark icon
	SLATE_ARGUMENT(UTexture2D*, ItemImage)

	SLATE_END_ARGS()

public:

	//Needed for every widget
	//Builds this widget and any of it's children
	void Construct(const FArguments& InArgs);

private:

	//Pointer to our parent HUD
	//To make sure HUD's lifetime is controlled elsewhere, use "weak" ptr.
	//HUD has "strong" pointer to Widget,
	//circular ownership would prevent/break self-destruction of hud/widget (cause memory leak).
	TWeakObjectPtr<class AonwardHUD> OwnerHUD;
			
	// A reference to the Slate Style used for this HUD's widgets.
	const struct FGlobalStyle* HUDStyle;

	//our watermark icon in brush form
	//const FSlateBrush WatermarkIcon = FSlateBrush(ESlateBrushDrawType::Image, FName("Texture2D'/Game/FieldGrassPackVol1/Textures/Ground/tex_grassground.tex_grassground'");
	FSlateBrush ItemBrush;
};
