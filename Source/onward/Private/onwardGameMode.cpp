// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "onwardCharacter.h"
#include "onwardGameInstance.h"
#include "onwardWorldSettings.h"
#include "onwardPlayerController.h"
#include "onwardHUD.h"
#include "onwardGameMode.h"

AonwardGameMode::AonwardGameMode()
{
	//hello world
	UE_LOG(HelloWorld, Log, TEXT("GameMode online; hello, world."))

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//set the PlayerController class
	PlayerControllerClass = AonwardPlayerController::StaticClass();

	//Set the hud class to use our custom HUD by default.
	this->HUDClass = AonwardHUD::StaticClass();
}



void AonwardGameMode::Tick(float DeltaSeconds)
{
	//increment world time once per frame
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());
	if (GI)
	{
		AonwardWorldSettings* WS = Cast<AonwardWorldSettings>(GetWorldSettings());
		if (WS)
		{
			float SecondsToAdd = DeltaSeconds * WS->GetRealWorldToInGameTimeConversionFactor();
			GI->GetWorldTime()->AddSeconds(SecondsToAdd);

			UE_LOG(LogWorldTime, Verbose, TEXT("time: %s  -  added %f sec to world time"), *(GI->GetWorldTime()->ToString(2)), SecondsToAdd);



			//move the sun
			if (WS->GetTempSun() != nullptr)
			{
				ADirectionalLight* Sun = WS->GetTempSun();

				FRotator Rot = FRotator(0, 0, 0);

				// Y  Z  X
				// P  Y  R

				Rot.Yaw = -90.0;
				Rot.Pitch = 360.0 * GI->GetWorldTime()->GetTimeOfDay() * -1 + 90.0;

				Sun->SetActorRotation(Rot);
			}
			else
			{
				UE_LOG(LogWorldTime, Verbose, TEXT("could not find a sun to move."));
			}
		}
	}



	Super::Tick(DeltaSeconds);
}



void AonwardGameMode::Time(FString iTime)
{
	FString t = iTime.ToLower();
	uint8 TargetHour = 0;

	//set general time of day
	/*0600*/ if		 (t == "morning")	{ TargetHour = 6; }
	/*1000*/ else if (t == "day")		{ TargetHour = 10; }
	/*1200*/ else if (t == "midday")	{ TargetHour = 12; }
	/*1400*/ else if (t == "afternoon")	{ TargetHour = 14; }
	/*1800*/ else if (t == "evening")	{ TargetHour = 18; }
	/*2200*/ else if (t == "night")		{ TargetHour = 22; }
	/*2400*/ else if (t == "midnight")	{ TargetHour = 0; }

	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("%s::Time(): I don't understand what you mean by %s."), *GetName(), *iTime);
		return;
	}


	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());
	if (GI)
	{
		FTimestamp* WorldTime = GI->GetWorldTime();
		if (WorldTime == nullptr)
		{
			//log error
			UE_LOG(LogWorldTime, Error, TEXT("%s::%s() %s : unable to get WorldTime"), *(CURR_CLASS), *(CURR_FUNCTION), *(CURR_LINE));
			return;
		}

		FTimestamp TargetTime = FTimestamp(WorldTime->GetYear(), WorldTime->GetMonth(), WorldTime->GetDay(), TargetHour, 0, 0);

		//check to see if target time is in the past
		if (WorldTime->GetTotalSeconds() >= TargetTime.GetTotalSeconds())
		{
			//if so, we need to forward time to sometime tomorrow, not today
			TargetTime.AddTime(0, 0, 1, 0, 0, 0);
		}

		ForwardTimeTo(TargetTime);

		UE_LOG(LogWorldTime, Log, TEXT("%s::%s() %s : time has been advanced to %s"), *(CURR_CLASS), *(CURR_FUNCTION), *(CURR_LINE), *(TargetTime.ToString(0)));
	}
}



void AonwardGameMode::TimeRate(float iNewRate)
{
	AonwardWorldSettings* WS = Cast<AonwardWorldSettings>(GetWorldSettings());

	if (WS)
	{
		WS->SetRealWorldMinutesPerInGameDay(iNewRate); //validation is done in here
	}
	
	UE_LOG(LogWorldTime, Warning, TEXT("Could not change time rate: could not find WorldSettings."));
}



void AonwardGameMode::TimeSet(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		FTimestamp TargetTime = FTimestamp(iYear, iMonth, iDay, iHour, iMinute, iSecond);

		if (TargetTime.GetTotalSeconds() > GI->GetWorldTime()->GetTotalSeconds()) //this is all the validation we need
		{
			ForwardTimeTo(TargetTime);

			UE_LOG(LogWorldTime, Log, TEXT("changed world time to %s"), *(GI->GetWorldTime()->ToString(1)));
		}
		else
		{
			UE_LOG(LogWorldTime, Warning, TEXT("Could not set time: could not find GameInstance."));
		}
	}
}



void AonwardGameMode::TimeAdd(uint32 iYear, uint8 iMonth, uint8 iDay, uint8 iHour, uint8 iMinute, uint8 iSecond)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		ForwardTimeTo(FTimestamp(GI->GetWorldTime()->GetTotalSeconds() + FTimestamp(iYear, iMonth, iDay, iHour, iMinute, iSecond).GetTotalSeconds(), GI->GetWorldTime()->GetRemainder()));
	}
	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: could not find GameInstance."));
		return;
	}
}



void AonwardGameMode::ForwardTimeTo(FTimestamp iTargetTime)
{
	UonwardGameInstance* GI = Cast<UonwardGameInstance>(GetGameInstance());

	if (GI)
	{
		FTimestamp* WorldTime = GI->GetWorldTime();

		//make sure this is indeed in the future
		if (iTargetTime.GetTotalSeconds() > WorldTime->GetTotalSeconds())
		{
			UE_LOG(LogWorldTime, Log, TEXT("Forwarding time to %s"), *(iTargetTime.ToString()));

			while (iTargetTime.GetTotalSeconds() > WorldTime->GetTotalSeconds())
			{
				//add random amount between 1 month and 1 year
				float SecondsToAdd = FMath::FRandRange(GAME_SECONDS_IN_MONTH, GAME_SECONDS_IN_YEAR);

				//overshoot detection
				if (WorldTime->GetTotalSeconds() + SecondsToAdd > iTargetTime.GetTotalSeconds())
				{
					//we overshot; clamp
					WorldTime->AddSeconds(iTargetTime.GetTotalSeconds() - WorldTime->GetTotalSeconds());
				}
				else
				{
					WorldTime->AddSeconds(SecondsToAdd);
				}

				UE_LOG(LogWorldTime, Log, TEXT("    added %f seconds, time is now %s"), SecondsToAdd, *(WorldTime->ToString()));
			}
		}
		else
		{
			UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: cannot go backwards in time."));
			return;
		}
	}
	else
	{
		UE_LOG(LogWorldTime, Warning, TEXT("Could not forward time: could not find GameInstance."));
		return;
	}
}



void AonwardGameMode::ToggleCrosshair()
{
	Cast<AonwardHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ToggleCrosshair();
}

void AonwardGameMode::ToggleDebugBars()
{
	Cast<AonwardHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ToggleDebugBars();
}

void AonwardGameMode::ToggleDebugOverlay()
{
	Cast<AonwardHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->ToggleDebugOverlay();
}

void AonwardGameMode::SetDebugOverlayDrawDistance(float NewDrawDistance)
{
	Cast<AonwardHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->SetPostRenderForDrawRange(NewDrawDistance);
}




void AonwardGameMode::DoTests()
{
	UE_LOG(HelloWorld, Warning, TEXT("    ----------------"));
	UE_LOG(HelloWorld, Warning, TEXT("     Starting tests"));
	UE_LOG(HelloWorld, Warning, TEXT("    ----------------"));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("multiplication:"));
	FTimestamp c1;
	c1 = FTimestamp(1, 1, 1, 1, 1, 1);
	int c2 = 0;
	int c3 = 4;
	float c4 = 2.0;
	double c5 = 10.5;
	UE_LOG(HelloWorld, Log, TEXT("%s * %d = %s"), *(c1.ToString()), c2, *((c1 * c2).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s * %d = %s"), *(c1.ToString()), c3, *((c1 * c3).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s * %f = %s"), *(c1.ToString()), c4, *((c1 * c4).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s * %f = %s"), *(c1.ToString()), c5, *((c1 * c5).ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("division:"));
	FTimestamp d1;
	d1 = FTimestamp(8, 4, 16, 8, 32, 32);
	int d2 = 0;
	int d3 = 4;
	float d4 = 2.0;
	double d5 = 8.4;
	UE_LOG(HelloWorld, Log, TEXT("%s / %d = %s"), *(d1.ToString()), d2, *((d1 / d2).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s / %d = %s"), *(d1.ToString()), d3, *((d1 / d3).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s / %f = %s"), *(d1.ToString()), d4, *((d1 / d4).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s / %f = %s"), *(d1.ToString()), d5, *((d1 / d5).ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("addition:"));
	FTimestamp a1, a2;
	a1 = FTimestamp(1, 0, 1, 0, 1, 0);
	a2 = FTimestamp(0, 1, 0, 1, 0, 1);
	UE_LOG(HelloWorld, Log, TEXT("%s + %s = %s"), *(a1.ToString()), *(a2.ToString()), *((a1 + a2).ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("subtraction:"));
	FTimestamp b1, b2;
	b1 = FTimestamp(1, 1, 1, 1, 1, 1);
	b2 = FTimestamp(0, 1, 0, 1, 0, 1);
	UE_LOG(HelloWorld, Log, TEXT("%s - %s = %s"), *(b1.ToString()), *(b2.ToString()), *((b1 - b2).ToString()));
	FTimestamp b3, b4;
	b3 = FTimestamp(6, 5, 4, 3, 2, 1);
	b4 = FTimestamp(1, 1, 1, 1, 1, 1);
	UE_LOG(HelloWorld, Log, TEXT("%s - %s = %s"), *(b3.ToString()), *(b4.ToString()), *((b3 - b4).ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s - %s = %s"), *(b4.ToString()), *(b3.ToString()), *((b4 - b3).ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("less than:"));
	FTimestamp e1, e2;
	e1 = FTimestamp(1, 0, 0, 0, 0, 0);
	e2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString eb1 = e1 < e2 ? "true" : "false";
	FString eb2 = e2 < e1 ? "true" : "false";
	FString eb3 = e1 < e1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s < %s = %s"), *(e1.ToString()), *(e2.ToString()), *(eb1));
	UE_LOG(HelloWorld, Log, TEXT("%s < %s = %s"), *(e2.ToString()), *(e1.ToString()), *(eb2));
	UE_LOG(HelloWorld, Log, TEXT("%s < %s = %s"), *(e1.ToString()), *(e1.ToString()), *(eb3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("less than or equal to:"));
	FTimestamp g1, g2;
	g1 = FTimestamp(1, 0, 0, 0, 0, 0);
	g2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString gb1 = g1 <= g2 ? "true" : "false";
	FString gb2 = g2 <= g1 ? "true" : "false";
	FString gb3 = g1 <= g1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s <= %s = %s"), *(g1.ToString()), *(g2.ToString()), *(gb1));
	UE_LOG(HelloWorld, Log, TEXT("%s <= %s = %s"), *(g2.ToString()), *(g1.ToString()), *(gb2));
	UE_LOG(HelloWorld, Log, TEXT("%s <= %s = %s"), *(g1.ToString()), *(g1.ToString()), *(gb3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("greater than:"));
	FTimestamp f1, f2;
	f1 = FTimestamp(1, 0, 0, 0, 0, 0);
	f2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString fb1 = f1 > f2 ? "true" : "false";
	FString fb2 = f2 > f1 ? "true" : "false";
	FString fb3 = f1 > f1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s > %s = %s"), *(f1.ToString()), *(f2.ToString()), *(fb1));
	UE_LOG(HelloWorld, Log, TEXT("%s > %s = %s"), *(f2.ToString()), *(f1.ToString()), *(fb2));
	UE_LOG(HelloWorld, Log, TEXT("%s > %s = %s"), *(f1.ToString()), *(f1.ToString()), *(fb3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("greater than or equal to:"));
	FTimestamp h1, h2;
	h1 = FTimestamp(1, 0, 0, 0, 0, 0);
	h2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString hb1 = h1 >= h2 ? "true" : "false";
	FString hb2 = h2 >= h1 ? "true" : "false";
	FString hb3 = h1 >= h1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s >= %s = %s"), *(h1.ToString()), *(h2.ToString()), *(hb1));
	UE_LOG(HelloWorld, Log, TEXT("%s >= %s = %s"), *(h2.ToString()), *(h1.ToString()), *(hb2));
	UE_LOG(HelloWorld, Log, TEXT("%s >= %s = %s"), *(h1.ToString()), *(h1.ToString()), *(hb3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("equal to:"));
	FTimestamp i1, i2;
	i1 = FTimestamp(1, 0, 0, 0, 0, 0);
	i2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString ib1 = i1 == i2 ? "true" : "false";
	FString ib2 = i2 == i1 ? "true" : "false";
	FString ib3 = i1 == i1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s == %s = %s"), *(i1.ToString()), *(i2.ToString()), *(ib1));
	UE_LOG(HelloWorld, Log, TEXT("%s == %s = %s"), *(i2.ToString()), *(i1.ToString()), *(ib2));
	UE_LOG(HelloWorld, Log, TEXT("%s == %s = %s"), *(i1.ToString()), *(i1.ToString()), *(ib3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("not equal to:"));
	FTimestamp j1, j2;
	j1 = FTimestamp(1, 0, 0, 0, 0, 0);
	j2 = FTimestamp(0, 1, 0, 0, 0, 0);
	FString jb1 = j1 != j2 ? "true" : "false";
	FString jb2 = j2 != j1 ? "true" : "false";
	FString jb3 = j1 != j1 ? "true" : "false";
	UE_LOG(HelloWorld, Log, TEXT("%s != %s = %s"), *(j1.ToString()), *(j2.ToString()), *(jb1));
	UE_LOG(HelloWorld, Log, TEXT("%s != %s = %s"), *(j2.ToString()), *(j1.ToString()), *(jb2));
	UE_LOG(HelloWorld, Log, TEXT("%s != %s = %s"), *(j1.ToString()), *(j1.ToString()), *(jb3));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("plus equals:"));
	FTimestamp k1, k2, k3;
	k1 = FTimestamp(1, 0, 0, 0, 0, 0);
	k2 = FTimestamp(0, 1, 0, 0, 0, 0);
	k3 = k1;
	k3 += k2;
	UE_LOG(HelloWorld, Log, TEXT("%s += %s = %s"), *(k1.ToString()), *(k2.ToString()), *(k3.ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("minus equals:"));
	FTimestamp l1, l2, l3, l4, l5;
	l1 = FTimestamp(1, 0, 0, 0, 0, 0);
	l2 = FTimestamp(0, 1, 0, 0, 0, 0);
	l3 = FTimestamp(2, 0, 0, 0, 0, 0);
	l4 = l1;
	l4 -= l2;
	l5 = l1;
	l5 -= l3;
	UE_LOG(HelloWorld, Log, TEXT("%s -= %s = %s"), *(l1.ToString()), *(l2.ToString()), *(l4.ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s -= %s = %s"), *(l1.ToString()), *(l3.ToString()), *(l5.ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("times equals:"));
	FTimestamp m1, m2, m3, m4;
	m1 = FTimestamp(1, 0, 0, 0, 0, 0);
	m2 = FTimestamp(0, 4, 0, 0, 0, 0);
	m3 = m1;
	m3 *= 2;
	m4 = m2;
	m4 *= 4.5;
	UE_LOG(HelloWorld, Log, TEXT("%s *= 2 = %s"), *(m1.ToString()), *(m3.ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s *= 4.5 = %s"), *(m2.ToString()), *(m4.ToString()));

	UE_LOG(HelloWorld, Log, TEXT(" "));
	UE_LOG(HelloWorld, Warning, TEXT("divide equals:"));
	FTimestamp n1, n2, n3, n4;
	n1 = FTimestamp(1, 4, 2, 0, 0, 0);
	n2 = FTimestamp(16, 0, 0, 0, 0, 0);
	n3 = n1;
	n3 /= 3;
	n4 = n2;
	n4 /= 4.0;
	UE_LOG(HelloWorld, Log, TEXT("%s /= 3 = %s"), *(n1.ToString()), *(n3.ToString()));
	UE_LOG(HelloWorld, Log, TEXT("%s /= 4.0 = %s"), *(n2.ToString()), *(n4.ToString()));
}
