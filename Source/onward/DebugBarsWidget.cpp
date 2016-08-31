#include "onward.h"
#include "onwardCharacter.h"
#include "DebugBarsWidget.h"
#include "onwardHUD.h"

#define LOCTEXT_NAMESPACE "SDebugBarsWidget"
#define PADDING 6,4 // horiz, vert

void SDebugBarsWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;



	WorldTime.Bind(this, &SDebugBarsWidget::GetWorldTime);
	WorldSeason.Bind(this, &SDebugBarsWidget::GetWorldSeason);

	PlayerHealthString.Bind(this, &SDebugBarsWidget::GetPlayerHealthString);



	//HUDStyle = &FMenuStyles::Get().GetWidgetStyle<FGlobalStyle>("Global");

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////If the code below doesn't look like C++ to you it's because it (sort-of) isn't,
	/////Slate makes extensive use of the C++ Prerocessor(macros) and operator overloading,
	/////Epic is trying to make our lives easier, look-up the macro/operator definitions to see why.
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		//.Padding(FMargin(8))
		[
			SNew(SVerticalBox)

			// world time of day
			+ SVerticalBox::Slot() .Padding(PADDING)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SProgressBar)
					.Percent(this, &SDebugBarsWidget::GetWorldTimeOfDay)
				]
				+ SOverlay::Slot()
				.Padding(PADDING)
				[
					SNew(STextBlock)
					.Text(WorldTime)
					.ShadowColorAndOpacity(FLinearColor::Black)
					.ShadowOffset(FIntPoint(1, 1))
				]
			]

			// world time of year
			+ SVerticalBox::Slot() .Padding(PADDING)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SProgressBar)
					.Percent(this, &SDebugBarsWidget::GetWorldTimeOfYear)
				]
				+ SOverlay::Slot()
				.Padding(PADDING)
				[
					SNew(STextBlock)
					.Text(WorldSeason)
					.ShadowColorAndOpacity(FLinearColor::Black)
					.ShadowOffset(FIntPoint(1, 1))
				]
			]

			// player health
			+ SVerticalBox::Slot().Padding(PADDING)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SProgressBar)
					.Percent(this, &SDebugBarsWidget::GetPlayerHealthPercentage) // ! change this !
				]
				+ SOverlay::Slot()
				.Padding(PADDING)
				[
					SNew(STextBlock)
					.Text(GetPlayerHealthString())
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(1, 1))
				]
			]

//			SNew(STextBlock)
//			//.ShadowColorAndOpacity(FLinearColor::Black)
//			.ColorAndOpacity(FLinearColor::White)
//			//.ShadowOffset(FIntPoint(-1, 1))
//			.Font(FSlateFontInfo("Veranda", 12))
//			.Text(LOCTEXT("HelloSlate", "2016-04-22 21:14:32 Hello World"))
		]
	];
}

#undef LOCTEXT_NAMESPACE



FText SDebugBarsWidget::GetWorldTime() const
{ 
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI) { return FText::FromString(GI->GetWorldTime()->ToString()); }
	return FText::FromString("000-00-00 00:00:00");
}



TOptional<float> SDebugBarsWidget::GetWorldTimeOfDay() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI) { return GI->GetWorldTime()->GetTimeOfDay(); }
	return 0.5;
}



FText SDebugBarsWidget::GetWorldSeason() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI) { return FText::FromString(GI->GetWorldTime()->GetSeason()); }
	return FText::FromString("- no season -");
}



TOptional<float> SDebugBarsWidget::GetWorldTimeOfYear() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI) { return GI->GetWorldTime()->GetTimeOfYear(); }
	return 0.5;
}



FText SDebugBarsWidget::GetPlayerHealthString() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI)
	{
		APlayerController* PC = GI->GetFirstLocalPlayerController();
		FString Ret = "";
		Ret += FString::SanitizeFloat(Cast<AonwardCharacter>(PC->GetPawn())->GetHealthCurrent()); // this will crash if this cast fails for some reason
		Ret += " / ";
		Ret += FString::SanitizeFloat(Cast<AonwardCharacter>(PC->GetPawn())->GetHealthTotal());
		return FText::FromString(Ret);
	}
	return FText::FromString("    /    ");
}



TOptional<float> SDebugBarsWidget::GetPlayerHealthPercentage() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI)
	{
		APlayerController* PC = GI->GetFirstLocalPlayerController();
		return Cast<AonwardCharacter>(PC->GetPawn())->GetHealthCurrent() / Cast<AonwardCharacter>(PC->GetPawn())->GetHealthTotal();
	}
	return 0.5;
}
