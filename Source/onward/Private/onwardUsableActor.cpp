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

void AonwardUsableActor::OnBeginFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void AonwardUsableActor::OnEndFocus()
{
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}

void AonwardUsableActor::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(HelloWorld, Log, TEXT("%s: %s is trying to use me!"), *(GetName()), *(InstigatorPawn->GetName()));
}
