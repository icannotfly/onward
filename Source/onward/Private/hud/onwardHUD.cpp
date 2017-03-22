// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "DebugBarsWidget.h"
#include "WatermarkWidget.h"
#include "onwardCharacter.h"
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
	DebugBars->SetVisibility(EVisibility::HitTestInvisible);

	//watermark in the top-left corner
	WatermarkWidget = SNew(SWatermarkWidget).OwnerHUD(this);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(WatermarkWidget.ToSharedRef()));
	WatermarkWidget->SetVisibility(EVisibility::HitTestInvisible);

	//show PostRenderFor overlays by default
	bShowOverlays = true;
}

void AonwardHUD::DrawHUD()
{
	//draw PostRenderFor overlays
	if (bShowOverlays)
	{
		PostRenderedActors.Empty();
		FlushDebugStrings(GetWorld());

		//draw only for actors in range - needs to be done before Super()
		for (TActorIterator<AonwardCharacter> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
		{
			if (
				GetOwningPawn()
				&& *ActorIterator != Cast<AonwardCharacter>(GetOwningPawn())	//make sure we don't add ourself to the list
				&& FMath::Abs(FVector::Dist(GetOwningPawn()->GetActorLocation(), ActorIterator->GetActorLocation())) <= PostRenderForDrawRange	//only add if we're in range
				&& GetOwningPawn()->GetDotProductTo(Cast<AActor>(*ActorIterator)) > 0.f	//only add if we're facing	//TODO we need to check the dot product between the CAMERA and the target, not the player and the target, otherwise things get weird in thirdperson
				)
			{
				AddPostRenderedActor(Cast<AActor>(*ActorIterator));
			}
		}
	}

	Super::DrawHUD();

	//TODO call DrawActorOverlays() if it's not done automatically - once an actor passes into our drawsphere, they add themselves to this list

	//draw a simple crosshair
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
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
	if (DebugBars->GetVisibility() == EVisibility::HitTestInvisible)
	{
		DebugBars->SetVisibility(EVisibility::Collapsed);
	}
	else if (DebugBars->GetVisibility() == EVisibility::Collapsed)
	{
		DebugBars->SetVisibility(EVisibility::HitTestInvisible);
	}
	else
	{
		UE_LOG(LogExec, Error, TEXT("%s debug bars have unexpected visibility state"), *(CURR_FUNC_CALL));
	}
}

void AonwardHUD::ToggleDebugOverlay()
{
	bShowOverlays = !bShowOverlays;

	if (bShowOverlays)
	{
		UE_LOG(HelloWorld, Log, TEXT("debug overlays turned on"));
	}
	else
	{
		UE_LOG(HelloWorld, Log, TEXT("debug overlays turned off"));
	}
}

void AonwardHUD::SetPostRenderForDrawRange(float NewPostRenderForDrawRange)
{
	if (NewPostRenderForDrawRange <= 0.0)
	{
		UE_LOG(HelloWorld, Error, TEXT("%s Cannot set PostRenderForDrawRange to %f"), *(CURR_FUNC_CALL), NewPostRenderForDrawRange);
		if (NewPostRenderForDrawRange == 0.0)
		{
			UE_LOG(HelloWorld, Warning, TEXT("Please use ToggleDebugOverlay to turn the overlay off"));
		}
		return;
	}

	PostRenderForDrawRange = NewPostRenderForDrawRange;
	UE_LOG(HelloWorld, Log, TEXT("PostRenderForDrawRange is now %f"), NewPostRenderForDrawRange);
}
