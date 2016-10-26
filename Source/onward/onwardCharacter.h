// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "onwardCharacter.generated.h"

UCLASS(config=Game)
class AonwardCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;



public:
	AonwardCharacter();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	//override
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//override
	virtual void BeginPlay() override;

	//override
	virtual void PossessedBy(AController *NewController) override;

	//are we currently sprinting?
	UFUNCTION(BlueprintCallable, Category = "Movement") bool IsSprinting() const;

	//returns our current health
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetHealthCurrent() const;

	//returns our total health
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetHealthTotal() const;

	//override
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	//called when the character dies, handles everything related to death
	void HandleDeath();

	//misc test functions
	UFUNCTION(Reliable, Server, WithValidation)
	void MyServerFunction();
	void MyServerFunction_Implementation();
	bool MyServerFunction_Validate();


protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//called on mousewheel scroll up
	void Input_ScrollUp();

	//called on mousewheel scroll down
	void Input_ScrollDown();



private:

	//is camera in firstperson mode?
	bool bCameraIsFirstperson = false;

	//called when sprint key is pressed, asks pawn to start sprinting - note that the pawn can deny our request based on lack of energy or some terrain consideration or something
	void RequestStartSprinting();

	//called when sprint key is released, asks pawn to stop sprinting
	void RequestStopSprinting();

	//set sprinting, either start or stop depending on bNewSprinting
	void SetSprinting(bool bNewSprinting);

	//like SetSprinting(), but done on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSprinting(bool bNewSprinting);

	//has our controller or human requested a sprint?
	bool bWantsToSprint = false;

	//current health
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float HealthCurrent = 100.0;

	//total health
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float HealthTotal = 100.0;

	//are we alive?
	bool bIsAlive = false;
};

