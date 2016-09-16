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

			// spacer
			+ SVerticalBox::Slot().Padding(PADDING)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.Padding(PADDING)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("blank"," "))
				]
			]

			// player health
			+ SVerticalBox::Slot().Padding(PADDING)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
				[
					SNew(SProgressBar)
					.Percent(this, &SDebugBarsWidget::GetPlayerHealthPercentage)
					.FillColorAndOpacity(FSlateColor::FSlateColor(FLinearColor::Red))
				]
				+ SOverlay::Slot()
				.Padding(PADDING)
				[
					SNew(STextBlock)
					.Text(PlayerHealthString)
					.ShadowColorAndOpacity(FLinearColor::Black)
					.ShadowOffset(FIntPoint(1, 1))
				]
			]

			// spacer

			// prone/crouch/standing

			// walk/run/sprint

			// movement speed

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
		FString Ret = " ";

		APlayerController* PC = OwnerHUD->PlayerOwner;
		if (PC)
		{
			AonwardCharacter *C = Cast<AonwardCharacter>(PC->GetPawn());
			if(C)
			{
				Ret = "HP: ";
				Ret += FString::SanitizeFloat(C->GetHealthCurrent());
				Ret += " / ";
				Ret += FString::SanitizeFloat(C->GetHealthTotal());
			}
			else
			{
				Ret = "NO PAWN";
			}
		}
		else
		{
			Ret = "NO OWNER";
		}

		return FText::FromString(Ret);
	}
	return FText::FromString("    /    ");
}



TOptional<float> SDebugBarsWidget::GetPlayerHealthPercentage() const
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(OwnerHUD->GetGameInstance());
	if (GI)
	{
		APlayerController* PC = OwnerHUD->PlayerOwner;
		if (PC)
		{
			AonwardCharacter *C = Cast<AonwardCharacter>(PC->GetPawn());
			if (C)
			{
				return C->GetHealthCurrent() / C->GetHealthTotal();
			}
		}
	}
	return 0.5;
}
