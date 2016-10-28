// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardUsableActor.h"


// Sets default values
AonwardUsableActor::AonwardUsableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AonwardUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AonwardUsableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

