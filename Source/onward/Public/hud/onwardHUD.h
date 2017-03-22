// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "onwardHUD.generated.h"

class SDebugBarsWidget;
class SWatermarkWidget;

/**
 * 
 */
UCLASS()
class ONWARD_API AonwardHUD : public AHUD
{
	GENERATED_BODY()


	
public:
	AonwardHUD();

	//Reference to an SCompoundWidget, TSharedPtr adds to the refcount of MyUIWidget
	//MyUIWidget will not self-destruct as long as refcount > 0
	//MyUIWidget refcount will be (refcout-1) if HUD is destroyed.
	TSharedPtr<SDebugBarsWidget> DebugBars;

	//watermark in top left-corner
	TSharedPtr<SWatermarkWidget> WatermarkWidget;

	void BeginPlay();

	//called once per frame to do the actual hud drawing
	virtual void DrawHUD() override;

	//toggle visibilty of the crosshair
	UFUNCTION() void ToggleCrosshair();

	//toggle visibilty of the debug bars
	UFUNCTION() void ToggleDebugBars();

	//toggle visibility of debug overlay
	UFUNCTION() void ToggleDebugOverlay();

	//sets PostRenderForDrawRange
	void SetPostRenderForDrawRange(float NewPostRenderForDrawRange);



private:

	//should we show the crosshair?
	bool bShowCrosshair = true;

	//timer for automatically updating debug overlays
	FTimerHandle UpdateDebugOverlays_TimerHandle;

	//seeks out other entities around us, adds them to the PostRenderFor draw queue if they should be in it, and prunes old objects from the list
	void ManageDebugOverlayTargets();

	//objects within this range will have their PostRenderFors called
	float PostRenderForDrawRange = 1500.f;	//TODO this whole process should be done with a IsOverlapping applied to a hitsphere that's parented to the player, otherwise we're checking against a vast number of entities per frame to see if they're in range
};
