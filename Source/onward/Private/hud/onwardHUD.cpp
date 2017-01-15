// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "DebugBarsWidget.h"
#include "onwardHUD.h"



AonwardHUD::AonwardHUD()
{

}

void AonwardHUD::BeginPlay()
{
	//So far only TSharedPtr<SMyUIWidget> has been created, now create the actual object.
	//Create a SMyUIWidget on heap, our MyUIWidget shared pointer provides handle to object
	//Widget will not self-destruct unless the HUD's SharedPtr (and all other SharedPtrs) destruct first.
	DebugBars = SNew(SDebugBarsWidget).OwnerHUD(this);

	//Pass our viewport a weak ptr to our widget
	//Viewport's weak ptr will not give Viewport ownership of Widget
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(DebugBars.ToSharedRef()));

	//Set widget's properties as visible (sets child widget's properties recursively)
	DebugBars->SetVisibility(EVisibility::Visible);
}

void AonwardHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	//draw a simple crosshair
	if(bShowCrosshair)
	{
		DrawRect(
			FLinearColor(1.0, 1.0, 1.0, 0.25),
			FMath::RoundToFloat(ViewportSize.X / 2.0) - 1,
			FMath::RoundToFloat(ViewportSize.Y / 2.0) - 1,
			2.0,
			2.0
		);
	}
}

void AonwardHUD::ToggleCrosshair()
{
	bShowCrosshair = !bShowCrosshair;
}

void AonwardHUD::ToggleDebugBars()
{
	if (DebugBars->GetVisibility() == EVisibility::Visible)
	{
		DebugBars->SetVisibility(EVisibility::Collapsed);
	}
	else if (DebugBars->GetVisibility() == EVisibility::Collapsed)
	{
		DebugBars->SetVisibility(EVisibility::Visible);
	}
	else
	{
		UE_LOG(LogExec, Error, TEXT("%s debug bars have unexpected visibility state"), *(CURR_FUNC_CALL));
	}
}
