// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// from https://wiki.unrealengine.com/Slate_Style_Sets_Part_2

#include "onward.h" 
#include "Public/hud/styles/GlobalDebugStyle.h" 

void FGlobalStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
}

const FName FGlobalStyle::TypeName = TEXT("FGlobalStyle");

const FName FGlobalStyle::GetTypeName() const
{
	static const FName TypeName = TEXT("FGlobalStyle");
	return TypeName;
}

const FGlobalStyle& FGlobalStyle::GetDefault()
{
	static FGlobalStyle Default;
	return Default;
}

UGlobalDebugStyle::UGlobalDebugStyle(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}
