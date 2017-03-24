// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "Public/hud/styles/DebugStyles.h"

//Custom implementation of the Default Game Module. 
// from https://wiki.unrealengine.com/Slate_Style_Sets_Part_2
class FonwardGameModule : public FDefaultGameModuleImpl
{
	// Called whenever the module is starting up. In here, we unregister any style sets 
	// (which may have been added by other modules) sharing our 
	// style set's name, then initialize our style set. 
	virtual void StartupModule() override
	{
		//Hot reload hack
		FSlateStyleRegistry::UnRegisterSlateStyle(FDebugStyles::GetStyleSetName());
		FDebugStyles::Initialize();
	}

	// Called whenever the module is shutting down. Here, we simply tell our MenuStyles to shut down.
	virtual void ShutdownModule() override
	{
		FDebugStyles::Shutdown();
	}

};

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, onward, "onward" );
 
DEFINE_LOG_CATEGORY(HelloWorld);
DEFINE_LOG_CATEGORY(LogWorldTime);
DEFINE_LOG_CATEGORY(LogCombat);
DEFINE_LOG_CATEGORY(LogCharacterMovement);
DEFINE_LOG_CATEGORY(LogCharacterVitals);
