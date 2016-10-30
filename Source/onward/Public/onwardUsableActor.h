// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "onwardUsableActor.generated.h"

UCLASS()
class ONWARD_API AonwardUsableActor : public AActor
{
	GENERATED_BODY()
		


public:	

	// Sets default values for this actor's properties
	AonwardUsableActor(const class FObjectInitializer& ObjectInitializer);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//called when the player starts looking at us
	virtual void OnBeginFocus();

	//called when the player stops looking at us :(
	virtual void OnEndFocus();

	//called when the player interacts with us
	virtual void OnUsed(APawn* InstigatorPawn);

	//returns a const pointer to the mesh component
	UStaticMeshComponent* GetMeshComponent() const;

	
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh") UStaticMeshComponent* MeshComponent;
};
