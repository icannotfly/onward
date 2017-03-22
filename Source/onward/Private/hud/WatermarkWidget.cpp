#include "onward.h"
#include "WatermarkWidget.h"
#include "onwardHUD.h"

#define LOCTEXT_NAMESPACE "SWatermarkWidget"

void SWatermarkWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;

	//ItemBrush.SetResourceObject(InArgs._ItemImage); //Texture2D'/Game/FieldGrassPackVol1/Textures/Ground/tex_grassground.tex_grassground'
	//ItemBrush.ImageSize.X = InArgs._ItemImage->GetSurfaceWidth();
	//ItemBrush.ImageSize.Y = InArgs._ItemImage->GetSurfaceHeight();
	//ItemBrush.DrawAs = ESlateBrushDrawType::Image;


	ChildSlot
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	[
		//SNew(SImage)
		//.Image(&ItemBrush)

		SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.Padding(4)
		[
			SNew(STextBlock)
			.Text(FText::FromString("[x]"))
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(1, 1))
		]

		+ SHorizontalBox::Slot()
		.Padding(4)
		[
			SNew(STextBlock)
			.Text(FText::FromString("onward alpha 0 (built 2017-01-21)"))
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ShadowOffset(FIntPoint(1, 1))
		]
	];


	//WorldTime.Bind(this, &SWatermarkWidget::GetWorldTime);
	//WorldSeason.Bind(this, &SWatermarkWidget::GetWorldSeason);

	//PlayerHealthString.Bind(this, &SWatermarkWidget::GetPlayerHealthString);
	//PlayerStaminaString.Bind(this, &SWatermarkWidget::GetPlayerStaminaString);
	//PlayerManaString.Bind(this, &SWatermarkWidget::GetPlayerManaString);
	
	//PlayerMovementStatusString.Bind(this, &SWatermarkWidget::GetPlayerMovementStatusString);
	////PlayerMovementVelocity.Bind(this, &SWatermarkWidget::GetPlayerMovementVelocity);



	//HUDStyle = &FMenuStyles::Get().GetWidgetStyle<FGlobalStyle>("Global");


	/*
	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		//.Padding(FMargin(8))
		[

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			//.Padding(10)
			[
				SNew(STextBlock)
				.Text(FText::FromString("[x]"))
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(1, 1))
			]

			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString("onward alpha 0"))
				//.Font(FSlateFontInfo("Veranda", 12))
				.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 16))
				.ShadowColorAndOpacity(FLinearColor::Black)
				.ShadowOffset(FIntPoint(1, 1))
			]



			//button - test
			+ SHorizontalBox::Slot()
			[
				SNew(SBox)
				.WidthOverride(32)
				.HeightOverride(32)
				[
					SNew(SImage)
					.Image(&WatermarkIcon)



					////~~~ Clear Button / Trash Can ~~~
					//SAssignNew(ClearTileButton, SButton)
					//.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					//.HAlign(HAlign_Center)
					//.VAlign(VAlign_Center)
					//.ForegroundColor(FSlateColor::UseForeground())
					//[
					//	//Button Content Image
					//	TSharedRef<SWidget>(SNew(SImage).Image(
					//		FCoreStyle::Get().GetBrush("TrashCan")
					//	))
					//]
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
	*/
}

#undef LOCTEXT_NAMESPACE
