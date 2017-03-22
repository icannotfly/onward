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

	//create a timer for updating debug overlays
	GetWorldTimerManager().SetTimer(UpdateDebugOverlays_TimerHandle, this, &AonwardHUD::ManageDebugOverlayTargets, 0.5/* rate */, true);
	if (!bShowOverlays)
	{
		GetWorldTimerManager().PauseTimer(UpdateDebugOverlays_TimerHandle);
	}
}

void AonwardHUD::DrawHUD()
{
	Super::DrawHUD();

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

void AonwardHUD::ManageDebugOverlayTargets()
{
	UE_LOG(HelloWorld, Verbose, TEXT("%s called"), *(CURR_FUNC_CALL))

	//make sure we have a pawn to check distances against
	if (GetOwningPawn())
	{
		//check current list, remove ents that don't match criteria
		for (int32 i = 0; i < PostRenderedActors.Num(); i++)
		{
			if (
				PostRenderedActors[i] == Cast<AonwardCharacter>(GetOwningPawn()) //if it's us
				|| FMath::Abs(FVector::Dist(GetOwningPawn()->GetActorLocation(), PostRenderedActors[i]->GetActorLocation())) <= PostRenderForDrawRange //or if it's out of range
				)
			{
				//remove it
				RemovePostRenderedActor(PostRenderedActors[i]);
			}
		}

		//add ents to list if they should be in there and aren't already
		for (TActorIterator<AonwardCharacter> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
		{
			if (
				*ActorIterator != Cast<AonwardCharacter>(GetOwningPawn())	//make sure we don't add ourself to the list
				&& FMath::Abs(FVector::Dist(GetOwningPawn()->GetActorLocation(), ActorIterator->GetActorLocation())) <= PostRenderForDrawRange	//only add if we're in range
				//&& GetOwningPawn()->GetDotProductTo(Cast<AActor>(*ActorIterator)) > 0.f	//only add if we're facing	//TODO we need to check the dot product between the CAMERA and the target, not the player and the target, otherwise things get weird in thirdperson
				)
			{
				AddPostRenderedActor(Cast<AActor>(*ActorIterator));
			}
		}
	}
	else
	{
		UE_LOG(HelloWorld, Error, TEXT("%s no OwningPawn, cannot continue"), *(CURR_FUNC_CALL));
	}
}

void AonwardHUD::ToggleDebugOverlay()
{
	bShowOverlays = !bShowOverlays;

	if (bShowOverlays)
	{
		UE_LOG(HelloWorld, Log, TEXT("debug overlays turned on"));
		GetWorldTimerManager().UnPauseTimer(UpdateDebugOverlays_TimerHandle);
	}
	else
	{
		UE_LOG(HelloWorld, Log, TEXT("debug overlays turned off"));
		GetWorldTimerManager().PauseTimer(UpdateDebugOverlays_TimerHandle);
		PostRenderedActors.Empty();
		FlushDebugStrings(GetWorld());
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
