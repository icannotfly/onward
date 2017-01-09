// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "onwardHUD.generated.h"

class SDebugBarsWidget;

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

	void BeginPlay();

	//called once per frame to do the actual hud drawing
	virtual void DrawHUD() override;
};
