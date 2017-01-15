// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "UnrealNetwork.h"
#include "onwardGameInstance.h" //only temporary for testing to see if the world time actually is being replicated
#include "items/onwardUsableActor.h"
#include "character/onwardCharacterMovementComponent.h"
#include "character/onwardCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AonwardCharacter

AonwardCharacter::AonwardCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UonwardCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	// Adjust jump to make it less floaty -- looman
	MoveComp->GravityScale = 1.5f;
	MoveComp->JumpZVelocity = 620;
	MoveComp->bCanWalkOffLedgesWhenCrouching = true;
	MoveComp->MaxWalkSpeedCrouched = 200;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	MaxUseDistance = 800;
	bHasNewFocus = true;

	SprintingSpeedModifier = 1.5;
	WalkingSpeedModifier = 0.4;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AonwardCharacter::SetupPlayerInputComponent(class UInputComponent* iInputComponent)
{
	// Set up gameplay key bindings
	check(iInputComponent);
	iInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	iInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	iInputComponent->BindAxis("MoveForward", this, &AonwardCharacter::MoveForward);
	iInputComponent->BindAxis("MoveRight", this, &AonwardCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	iInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	iInputComponent->BindAxis("TurnRate", this, &AonwardCharacter::TurnAtRate);
	iInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	iInputComponent->BindAxis("LookUpRate", this, &AonwardCharacter::LookUpAtRate);

	// handle touch devices
	iInputComponent->BindTouch(IE_Pressed, this, &AonwardCharacter::TouchStarted);
	iInputComponent->BindTouch(IE_Released, this, &AonwardCharacter::TouchStopped);

	//camera movement
	iInputComponent->BindAction("CameraMoveIn", IE_Pressed, this, &AonwardCharacter::Input_ScrollUp);
	iInputComponent->BindAction("CameraMoveOut", IE_Pressed, this, &AonwardCharacter::Input_ScrollDown);

	//movement - sprint
	iInputComponent->BindAction("Sprint", IE_Pressed, this, &AonwardCharacter::RequestStartSprinting);
	iInputComponent->BindAction("Sprint", IE_Released, this, &AonwardCharacter::RequestStopSprinting);

	//movement - walk
	iInputComponent->BindAction("Walk", IE_Pressed, this, &AonwardCharacter::RequestStartWalking);
	iInputComponent->BindAction("Walk", IE_Released, this, &AonwardCharacter::RequestStopWalking);

	//interaction - use
	iInputComponent->BindAction("Use", IE_Pressed, this, &AonwardCharacter::Use);
}



void AonwardCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AonwardCharacter, HealthCurrent);
	DOREPLIFETIME(AonwardCharacter, HealthTotal);
}



void AonwardCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//temp, for debugging
	//TODO delete me
	if (Controller && Controller->IsLocalController()) //pretty sure this is a test to see if we're human
	{
		if (Role == ROLE_Authority)
		{
			FString derp;
			derp = "AUTH:    ";
			derp += Cast<UonwardGameInstance>(GetGameInstance())->GetWorldTime()->ToString();
			derp += "    ";
			derp += GetName();
			GEngine->AddOnScreenDebugMessage(-1, -1, FColor::White, *(derp));
		}
	}

	//sprinting
	if (bWantsToSprint && !IsSprinting())
	{
		SetSprinting(true);
	}

	//walking
	if (bWantsToWalk && !IsWalking())
	{
		SetWalking(true);
	}

	//debug
	if (IsSprinting())
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::White, "sprinting");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, -1, FColor::White, "not sprinting");
	}
}



void AonwardCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AonwardCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AonwardCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AonwardCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AonwardCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if(Role == ROLE_Authority)
		{
			MyServerFunction();
		}

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AonwardCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}



void AonwardCharacter::BeginPlay()
{
	Super::BeginPlay();

	//attach camera to thirdperson target
	GetCameraBoom()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,"ThirdpersonCameraTarget");
}



void AonwardCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);

	//not sure if this is the optimal place to do this but whatevs
	bIsAlive = true;
}



void AonwardCharacter::Input_ScrollUp()
{
	if (GetCameraBoom()->TargetArmLength > 100.0)
	{
		bCameraIsFirstperson = false;
		GetCameraBoom()->TargetArmLength -= 100.0;
	}
	else
	{
		//zoomed in all the way
		bCameraIsFirstperson = true;

		GetCameraBoom()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "eyes");
		GetCameraBoom()->TargetArmLength = 0;
		
		//and move camera and character together
		bUseControllerRotationPitch = false; //enable for funky weird moonwalking
		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;
	}
}



void AonwardCharacter::Input_ScrollDown()
{
	GetCameraBoom()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "ThirdpersonCameraTarget");

	//move camera seperately of character
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false; //set true to make character yaw with camera

	if (GetCameraBoom()->TargetArmLength < 1000.f)
	{
		bCameraIsFirstperson = false;
		GetCameraBoom()->TargetArmLength += 100.f;
	}
	else
	{
		//zoomed all the way out
		GetCameraBoom()->TargetArmLength = 1000.f;
	}
}



void AonwardCharacter::RequestStartSprinting()
{
	SetSprinting(true);
}

void AonwardCharacter::RequestStopSprinting()
{
	SetSprinting(false);
}

void AonwardCharacter::SetSprinting(bool bNewSprinting)
{
	bWantsToSprint = bNewSprinting;

	//un-prone, un-crouch, all that stuff
	//stop in-progress actions

	FString in = bWantsToSprint ? "true" : "false";
	UE_LOG(LogPlayerMovement, Warning, TEXT("(SPRINT) input is %s "), *(in));

	if (Role < ROLE_Authority)
	{
		Server_SetSprinting(bNewSprinting);
	}
}

void AonwardCharacter::Server_SetSprinting_Implementation(bool bNewSprinting)
{
	SetSprinting(bNewSprinting);

	if (bNewSprinting)
	{
		UE_LOG(LogPlayerMovement, Log, TEXT("%s starting to sprint"), *(GetName()));
	}
	else
	{
		UE_LOG(LogPlayerMovement, Log, TEXT("%s stopping sprinting"), *(GetName()));
	}
}

bool AonwardCharacter::Server_SetSprinting_Validate(bool bNewSprinting)
{
	//TODO check to see if we have enough energy to start sprinting, which means //TODO implement energy
	return true;		//is this where we should check to see if we have enough energy?
}

bool AonwardCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return bWantsToSprint;		//TODO and other conditions, check looman's code line 366
}



void AonwardCharacter::RequestStartWalking()
{
	SetWalking(true);
}

void AonwardCharacter::RequestStopWalking()
{
	SetWalking(false);
}

void AonwardCharacter::SetWalking(bool bNewWalking)
{
	bWantsToWalk = bNewWalking;

	//un-prone, un-crouch, all that stuff
	//stop in-progress actions

	FString in = bWantsToWalk ? "true" : "false";
	UE_LOG(LogPlayerMovement, Warning, TEXT("(WALK) input is %s "), *(in));

	if (Role < ROLE_Authority)
	{
		Server_SetWalking(bNewWalking);
	}
}

void AonwardCharacter::Server_SetWalking_Implementation(bool bNewWalking)
{
	SetWalking(bNewWalking);

	if (bNewWalking)
	{
		UE_LOG(LogPlayerMovement, Log, TEXT("%s starting to walk"), *(GetName()));
	}
	else
	{
		UE_LOG(LogPlayerMovement, Log, TEXT("%s stopping walking"), *(GetName()));
	}
}

bool AonwardCharacter::Server_SetWalking_Validate(bool bNewWalking)
{
	//TODO check to see if we have enough energy to start walking, which means //TODO implement energy
	return true;		//is this where we should check to see if we have enough energy?
}

bool AonwardCharacter::IsWalking() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return bWantsToWalk;		//TODO and other conditions, check looman's code line 366
}



float AonwardCharacter::GetHealthCurrent() const
{
	return HealthCurrent;
}

float AonwardCharacter::GetHealthTotal() const
{
	return HealthTotal;
}

float AonwardCharacter::GetStaminaCurrent() const
{
	return StaminaCurrent;
}

float AonwardCharacter::GetStaminaTotal() const
{
	return StaminaTotal;
}

float AonwardCharacter::GetManaCurrent() const
{
	return ManaCurrent;
}

float AonwardCharacter::GetManaTotal() const
{
	return ManaTotal;
}

float AonwardCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//calculate damage
	float FinalDamageAmount = DamageAmount;

	//apply damage
	if (FinalDamageAmount == 0)
	{
		//no change
		UE_LOG(LogCombat, Warning, TEXT("zero damage dealt"));
	}
	else
	{
		HealthCurrent -= FinalDamageAmount;

		//log the damage to the in-game-viewable console (hit tilde twice)
		FString message = GetName();
		message += " took ";
		message += FString::SanitizeFloat(FinalDamageAmount);
		message += " ";
		message += DamageEvent.DamageTypeClass != nullptr ? DamageEvent.DamageTypeClass->GetName() : "unknown";
		message += " damage (";
		message += FString::SanitizeFloat(0.0);
		message += " mitigated) from ";
		message += DamageCauser->GetName();

		UE_LOG(LogCombat, Log, TEXT("%s  Health now %f / %f"), *(message), HealthCurrent, HealthTotal);
		GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld())->ClientMessage(message);

		if (FinalDamageAmount > 0)
		{
			//damage
			if (HealthCurrent <= 0)
			{
				//death
				HealthCurrent = 0;

				HandleDeath();
			}
		}
		else
		{
			//healing
			if (HealthCurrent > HealthTotal)
			{
				//no overheal
				HealthCurrent = HealthTotal;
			}
		}
	}

	return FinalDamageAmount;
}

void AonwardCharacter::HandleDeath()
{
	bIsAlive = false;

	FString message = GetName();
	message += " died!";

	UE_LOG(LogCombat, Warning, TEXT("%s"), *(message));
	GetGameInstance()->GetLocalPlayers()[0]->GetPlayerController(GetWorld())->ClientMessage(message);
}



void AonwardCharacter::ChangeStamina(float iChangeAmount)
{
	StaminaCurrent += iChangeAmount;

	//check for overrun
	if (StaminaCurrent > StaminaTotal)
	{
		StaminaCurrent = StaminaTotal;
	}
	//check for underrun
	else if (StaminaCurrent < 0.0)
	{
		StaminaCurrent = 0.0;
		UE_LOG(LogCharacterVitals, Warning, TEXT(""));
	}
}



void AonwardCharacter::ChangeMana(float iChangeAmount)
{
	ManaCurrent += iChangeAmount;

	//check for overrun
	if (ManaCurrent > ManaTotal)
	{
		ManaCurrent = ManaTotal;
	}
	//check for underrun
	else if (ManaCurrent < 0.0)
	{
		//remove the underrun amount from stamina instead (too much thinking makes you pass out)
		ChangeStamina(
			ManaCurrent * 0.667 //mana-to-stamina conversion factor (TODO)
		);

		//clamp to 0
		ManaCurrent = 0.0;

		UE_LOG(LogCharacterVitals, Warning, TEXT(""));
	}
}



void AonwardCharacter::MyServerFunction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, -1, FColor::White, TEXT("hello there - called on client and executed on server."));
}



bool AonwardCharacter::MyServerFunction_Validate()
{
	return true; //lol
}



float AonwardCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}

float AonwardCharacter::GetWalkingSpeedModifier() const
{
	return WalkingSpeedModifier;
}



void AonwardCharacter::Use()
{
	//only allow to be called on server; if called on client, tell the server to try it
	if (Role == ROLE_Authority)
	{
		UE_LOG(LogInput, Log, TEXT("Use requested as server."));

		AonwardUsableActor* Usable = GetUsableInView();
		if (Usable)
		{	
			Usable->OnUsed(this);
		}
	}
	else
	{
		UE_LOG(LogInput, Log, TEXT("Use requested; asking server to Use()."));
		ServerUse();
	}
}



void AonwardCharacter::ServerUse_Implementation()
{
	UE_LOG(LogInput, Log, TEXT("Use request received by server."));
	Use();
}



bool AonwardCharacter::ServerUse_Validate()
{
	//TODO
	return true;
}



class AonwardUsableActor* AonwardCharacter::GetUsableInView()
{
	if (Controller == NULL || Controller == nullptr)
	{
		UE_LOG(LogInput, Warning, TEXT("%s::%s() at line (%s): %s could not find a controller."), *(CURR_CLASS), *(CURR_FUNCTION), *(CURR_LINE), *(GetName()));
		return nullptr;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);
	const FVector TraceStart = GetMesh()->GetSocketLocation("eyes");
	const FVector Direction = CameraRotation.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraseUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	//debug
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<AonwardUsableActor>(Hit.GetActor());
}
