// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
// thanks to https://wiki.unrealengine.com/Slate_Style_Sets_Part_2

#pragma once

#include "SlateBasics.h"

class FDebugStyles
{
public:

	// Initializes the value of MenuStyleInstance and registers it with the Slate Style Registry.
	static void Initialize();

	// Unregisters the Slate Style Set and then resets the MenuStyleInstance pointer.
	static void Shutdown();

	// Retrieves a reference to the Slate Style pointed to by MenuStyleInstance.
	static const class ISlateStyle& Get();

	// Retrieves the name of the Style Set.
	static FName GetStyleSetName();



private:
	
	// Creates the Style Set.
	static TSharedRef<class FSlateStyleSet> Create();

	// Singleton instance used for our Style Set.
	static TSharedPtr<class FSlateStyleSet> MenuStyleInstance;

};
