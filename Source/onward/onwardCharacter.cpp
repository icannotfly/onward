// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "onward.h"
#include "onwardCharacter.h"

//////////////////////////////////////////////////////////////////////////
// AonwardCharacter

AonwardCharacter::AonwardCharacter()
{
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
}

//////////////////////////////////////////////////////////////////////////
// Input

void AonwardCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AonwardCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AonwardCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AonwardCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AonwardCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AonwardCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AonwardCharacter::TouchStopped);

	//camera movement
	InputComponent->BindAction("CameraMoveIn", IE_Pressed, this, &AonwardCharacter::Input_ScrollUp);
	InputComponent->BindAction("CameraMoveOut", IE_Pressed, this, &AonwardCharacter::Input_ScrollDown);

	//movement - sprint
	InputComponent->BindAction("Sprint", IE_Pressed, this, &AonwardCharacter::Input_RequestSprintStart);
	InputComponent->BindAction("Sprint", IE_Released, this, &AonwardCharacter::Input_RequestSprintStop);
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
	GetCameraBoom()->SetupAttachment(GetMesh(), "ThirdpersonCameraTarget"/*, EAttachLocation::SnapToTarget*/);
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

		GetCameraBoom()->SetupAttachment(GetMesh(), "eyes"/*, EAttachLocation::SnapToTarget*/);
		GetCameraBoom()->TargetArmLength = 0;
		
		//and move camera and character together
		bUseControllerRotationPitch = false; //enable for funky weird moonwalking
		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;
	}
}



void AonwardCharacter::Input_ScrollDown()
{
	GetCameraBoom()->SetupAttachment(GetMesh(), "ThirdpersonCameraTarget"/*, EAttachLocation::SnapToTarget*/);

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



void AonwardCharacter::Input_RequestSprintStart()
{
	UE_LOG(HelloWorld, Log, TEXT("sprint start requested"));
}



void AonwardCharacter::Input_RequestSprintStop()
{
	UE_LOG(HelloWorld, Log, TEXT("sprint stop  requested"));
}



void AonwardCharacter::MyServerFunction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("hello there - called on client and executed on server."));
}



bool AonwardCharacter::MyServerFunction_Validate()
{
	return true; //lol
}
