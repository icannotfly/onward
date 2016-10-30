// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "onwardUsableActor.h"
#include "TestTorch.generated.h"



UCLASS()
class ONWARD_API ATestTorch : public AonwardUsableActor
{
	GENERATED_BODY()
	
	ATestTorch(const class FObjectInitializer& ObjectInitializer);
	
	

public:

	//ignites the torch on use
	virtual void OnUsed(APawn* InstigatorPawn) override;



private:

	//
	UPROPERTY(VisibleDefaultsOnly) UParticleSystemComponent* FlameParticleComponent;

	//
	UPROPERTY(VisibleDefaultsOnly) UAudioComponent* AudioComponent;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Torch|Effects") UParticleSystem* FlameEffect;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Torch|Effects") USoundCue* FlameSound;

	//returns true if the torch is lit
	UPROPERTY(Transient, ReplicatedUsing = OnRep_TorchLit) bool bIsTorchLit;

	//
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurnedOut) bool bHasBurnedOut;

	//must be marked with UFUNCTION() when used as an OnRep notify function
	UFUNCTION() void OnRep_TorchLit();

	//must be marked with UFUNCTION() when used as an OnRep notify function
	UFUNCTION() void OnRep_BurnedOut();

	//how long the torch will burn
	UPROPERTY(EditDefaultsOnly, Category = "Torch|Settings") float MaxBurnTime;

	//burn out
	void OnBurnedOut();

	//
	void SimulateFlameEffect();

	//
	void SimulateBurnOut();
};
