// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "onwardUsableActor.h"
#include "onwardTestItem.generated.h"



UCLASS()
class ONWARD_API AonwardTestItem : public AonwardUsableActor
{
	GENERATED_BODY()
	
	AonwardTestItem(const class FObjectInitializer& ObjectInitializer);


	
public:
	
	/* Activates the bomb fuze */
	virtual void OnUsed(APawn* InstigatorPawn) override;



private:

	UPROPERTY(VisibleDefaultsOnly) UParticleSystemComponent* ExplosionPCS;

	UPROPERTY(VisibleDefaultsOnly) UParticleSystemComponent* FuzePCS;

	UPROPERTY(VisibleDefaultsOnly) UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects") UParticleSystem* ExplosionFX;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects") UParticleSystem* FuzeFX;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects") USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Effects") USoundCue* FuzeSound;

	/* Is fuze lit and counting down */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_FuzeActive) bool bIsFuzeActive;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Exploded) bool bExploded;

	// Must be marked with UFUNCTION() when used as OnRep notify function
	UFUNCTION() void OnRep_FuzeActive();

	// Must be marked with UFUNCTION() when used as OnRep notify function
	UFUNCTION() void OnRep_Exploded();

	/* Initial time on the fuze */
	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings") float MaxFuzeTime;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings") float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings") float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Bomb|Settings") TSubclassOf<UDamageType> DamageType;

	/* Explode the bomb */
	void OnExplode();

	void SimulateFuzeFX();

	void SimulateExplosion();
};
