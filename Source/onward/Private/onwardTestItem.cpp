// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "onwardTestItem.h"



AonwardTestItem::AonwardTestItem(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FuzePCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Fuze"));
	FuzePCS->bAutoActivate = false;
	FuzePCS->bAutoDestroy = false;
	//FuzePCS->AttachParent = RootComponent;
	//FuzePCS->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	FuzePCS->SetupAttachment(RootComponent);

	ExplosionPCS = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Explosion"));
	ExplosionPCS->bAutoActivate = false;
	ExplosionPCS->bAutoDestroy = false;
	//ExplosionPCS->AttachParent = RootComponent;
	//ExplosionPCS->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	ExplosionPCS->SetupAttachment(RootComponent);

	AudioComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	AudioComp->bAutoDestroy = false;
	//AudioComp->AttachParent = RootComponent;
	//AudioComp->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	AudioComp->SetupAttachment(RootComponent);

	// Let the bomb be thrown and roll around
	GetMeshComponent()->SetSimulatePhysics(true);

	MaxFuzeTime = 5.0f;
	ExplosionDamage = 100;
	ExplosionRadius = 512;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bReplicateMovement = true;
}

void AonwardTestItem::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	if (!bIsFuzeActive)
	{
		// This will trigger the ActivateFuze() on the clients
		bIsFuzeActive = true;

		// Repnotify does not trigger on the server, so call the function here directly.
		SimulateFuzeFX();

		// Active the fuze to explode the bomb after several seconds
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AonwardTestItem::OnExplode, MaxFuzeTime, false);
	}
}


void AonwardTestItem::OnExplode()
{
	if (bExploded)
		return;

	// Notify the clients to simulate the explosion
	bExploded = true;

	// Run on server side
	SimulateExplosion();

	// Apply damage to player, enemies and environmental objects
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, DamageType, IgnoreActors, this, NULL);

	// TODO: Deal Damage to objects that support it
	// TODO: Apply radial impulse to supporting objects
	// TODO: Prepare to destroy self
}


void AonwardTestItem::OnRep_FuzeActive()
{
	if (bIsFuzeActive && !bExploded)
	{
		SimulateFuzeFX();
	}
}


void AonwardTestItem::OnRep_Exploded()
{
	if (bExploded)
	{
		SimulateExplosion();
	}
}


void AonwardTestItem::SimulateFuzeFX()
{
	if (FuzeSound)
	{
		AudioComp->SetSound(FuzeSound);
		AudioComp->FadeIn(0.25f);
	}
	if (FuzeFX)
	{
		FuzePCS->SetTemplate(FuzeFX);
		FuzePCS->ActivateSystem();
	}
}


void AonwardTestItem::SimulateExplosion()
{
	// First deactivate all running fuze effects
	FuzePCS->DeactivateSystem();
	AudioComp->Stop();

	GetMeshComponent()->SetVisibility(false, false);

	// Active all explosion effects
	if (ExplosionSound)
	{
		AudioComp->SetSound(ExplosionSound);
		AudioComp->Play();
	}
	if (ExplosionFX)
	{
		ExplosionPCS->SetTemplate(ExplosionFX);
		ExplosionPCS->ActivateSystem();
	}
}


void AonwardTestItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AonwardTestItem, bIsFuzeActive);
	DOREPLIFETIME(AonwardTestItem, bExploded);
}
