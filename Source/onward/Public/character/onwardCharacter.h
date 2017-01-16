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
	AonwardCharacter(const class FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//override
	virtual void Tick(float DeltaSeconds) override;

	//override
	virtual void BeginPlay() override;

	//override
	virtual void PossessedBy(AController *NewController) override;



	//==========================
	// vitals
	//==========================

	//are we currently sprinting?
	UFUNCTION(BlueprintCallable, Category = "Movement") bool IsSprinting() const;

	//are we currently walking? //TODO might want to replace these two with an enum that describes our movement state, or expand an existing enum
	UFUNCTION(BlueprintCallable, Category = "Movement") bool IsWalking() const;

	//returns our current health
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetHealthCurrent() const;

	//returns our total health
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetHealthTotal() const;

	//are we alive?
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") bool IsAlive() const;

	//returns our current stamina
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetStaminaCurrent() const;

	//returns our total stamina
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetStaminaTotal() const;
	
	//alters the amount of stamina this character currently has - does not modify total amount
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") void ChangeStamina(float iChangeAmount, bool bAllowPassingOut = false);

	//server version of ChangeStamina
	UFUNCTION(Server, Reliable, WithValidation) void Server_ChangeStamina(float iChangeAmount, bool bAllowPassingOut = false);

	//returns our current mana
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetManaCurrent() const;

	//returns our total mana
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") float GetManaTotal() const;

	//alters the amount of mana this character currently has - does not modify total amount
	UFUNCTION(BlueprintCallable, Category = "PlayerVitals") void ChangeMana(float iChangeAmount);

	//override
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	//called when the character dies, handles everything related to death
	void HandleDeath();

	

	//==========================
	// movement
	//==========================

	//returns the sprinting speed modifier
	float GetSprintingSpeedModifier() const;

	//returns the walking speed modifier
	float GetWalkingSpeedModifier() const;



	//==========================
	// world interaction
	//==========================

	//use whatever's in focus
	virtual void Use();

	//server's version of Use()
	UFUNCTION(Server, Reliable, WithValidation) void ServerUse();

	//returns the UsableActor we're currently looking at, if any. will return NULL or nullptr if none in view (idk which, try testing for nullptr)
	class AonwardUsableActor* GetUsableInView();

	

	//==========================
	// misc test functions
	//==========================

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

	//sprinting movement speed = base movement speed * SprintingSpeedModifier;
	UPROPERTY(EditDefaultsOnly, Category = "Movement") float SprintingSpeedModifier;

	//like SetSprinting(), but done on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetSprinting(bool bNewSprinting);

	//has our controller or human requested a sprint?
	bool bWantsToSprint = false;



	//called when walk key is pressed, asks pawn to start walking - note that the pawn can deny our request based on lack of energy or some terrain consideration or something
	void RequestStartWalking();

	//called when walk key is released, asks pawn to stop walking
	void RequestStopWalking();

	//set walking, either start or stop (depending on bNewWalking)
	void SetWalking(bool bNewWalking);

	//walking movement speed = base movement speed * WalkingSpeedModifier;
	UPROPERTY(EditDefaultsOnly, Category = "Movement") float WalkingSpeedModifier;

	//SetWalkinging() thats done on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetWalking(bool bNewWalking);

	//has our controller or human requested to walk?
	bool bWantsToWalk = false;



	//current health
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float HealthCurrent = 100.0;

	//total health
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float HealthTotal = 100.0;

	//current stamina
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float StaminaCurrent = 100.0;

	//total stamina
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float StaminaTotal = 100.0;

	//current mana
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float ManaCurrent = 100.0;

	//total mana
	UPROPERTY(EditDefaultsOnly, Category = "PlayerVitals", Replicated) float ManaTotal = 100.0;

	//are we alive?
	bool bIsAlive = false;
	
	//distance beyone which objects will be considered out of our "reach" and cannot be used
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction") float MaxUseDistance;

	//true only for the first frame in which we have changed our focus to a new UsableActor
	bool bHasNewFocus;

	//the actor we're currently staring at like a creepy motherfucker
	class AonwardUsableActor* FocusedUsableActor;
};

