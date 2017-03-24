// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// thanks to https://wiki.unrealengine.com/Slate_Style_Sets_Part_2

#include "onward.h"
#include "Public/hud/styles/DebugStyles.h"
#include "SlateGameResources.h" 

TSharedPtr<FSlateStyleSet> FDebugStyles::MenuStyleInstance = NULL;

void FDebugStyles::Initialize()
{
	if (!MenuStyleInstance.IsValid())
	{
		MenuStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*MenuStyleInstance);
	}
}

void FDebugStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*MenuStyleInstance);
	ensure(MenuStyleInstance.IsUnique());
	MenuStyleInstance.Reset();
}

FName FDebugStyles::GetStyleSetName()
{
	static FName StyleSetName(TEXT("DebugStyles"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FDebugStyles::Create()
{
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FDebugStyles::GetStyleSetName(), "/Game/ui/styles", "/Game/ui/styles");
	return StyleRef;
}

const ISlateStyle& FDebugStyles::Get()
{
	return *MenuStyleInstance;
}
