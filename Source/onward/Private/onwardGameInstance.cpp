// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardWorldSettings.h"
#include "UnrealNetwork.h" //for replication macros
#include "onwardGameInstance.h"



void UonwardGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//replicate to everyone
	DOREPLIFETIME(UonwardGameInstance, WorldTime);

	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("hello there"));
	}
}
