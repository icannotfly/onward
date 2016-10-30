// Fill out your copyright notice in the Description page of Project Settings.

#include "onward.h"
#include "TestTorch.h"



ATestTorch::ATestTorch(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{ 
	FlameParticleComponent = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("Flame"));
	FlameParticleComponent->bAutoActivate = false;
	FlameParticleComponent->bAutoDestroy = false;
	FlameParticleComponent->SetupAttachment(RootComponent);
	//FlameParticleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	AudioComponent = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bAutoDestroy = false;
	AudioComponent->SetupAttachment(RootComponent);
	//AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	GetMeshComponent()->SetSimulatePhysics(true);

	MaxBurnTime = 10.f;

	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);		//TODO do we need this?
	bReplicates = true;
	bReplicateMovement = true;
}

void ATestTorch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestTorch, bIsTorchLit);
	DOREPLIFETIME(ATestTorch, bHasBurnedOut);
}


void ATestTorch::OnUsed(APawn* InstigatorPawn)
{
	Super::OnUsed(InstigatorPawn);

	if (!bIsTorchLit)
	{
		bIsTorchLit = true;

		SimulateFlameEffect();

		FTimerHandle BurnoutTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(BurnoutTimerHandle, this, &ATestTorch::OnBurnedOut, MaxBurnTime, false);
	}
}

void ATestTorch::OnRep_TorchLit()
{
	if (bIsTorchLit && !bHasBurnedOut)
	{

	}
}

void ATestTorch::OnRep_BurnedOut()
{
	if (bHasBurnedOut)
	{
		SimulateBurnOut();
	}
}

void ATestTorch::OnBurnedOut()
{
	if (bHasBurnedOut)
	{
		return;
	}

	//setting this will notify clients of a change because we "bound" it in GetLifetimeReplicatedProps()
	bHasBurnedOut = true;

	SimulateBurnOut();
}

void ATestTorch::SimulateFlameEffect()
{
	if (FlameEffect)
	{
		FlameParticleComponent->SetTemplate(FlameEffect);
		FlameParticleComponent->ActivateSystem();
	}

	if (FlameSound)
	{
		AudioComponent->SetSound(FlameSound);
		AudioComponent->FadeIn(0.75);
	}
}

void ATestTorch::SimulateBurnOut()
{
	FlameParticleComponent->DeactivateSystem();
	AudioComponent->FadeOut(1.0, 0.f);

	//GetMeshComponent()->SetVisibility(false, false);		//we'll keep it around for now
}
