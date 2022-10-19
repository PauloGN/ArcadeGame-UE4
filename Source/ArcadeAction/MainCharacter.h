// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName, "Normal"),
	EMS_Sprinting UMETA(DisplayName, "Sprinting"),
	EMS_Dead UMETA(DisplayName, "Dead"),
	EMS_MAX UMETA(DisplayName, "DefaultMAX")
};


UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName, "Normal"),
	ESS_BelowMinimum UMETA(DisplayName, "BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName, "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName, "Exhausted"),
	ESS_MAX UMETA(DisplayName, "DefaultMAX")
};



UCLASS()
class ARCADEACTION_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	/***
	* aggregation
	***/


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlappingItem;

	TArray<FVector> PickUpLocations;

	UFUNCTION(BlueprintCallable)
	void ShowPickUpLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/**Base turns rates to scale turning functions for the camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UParticleSystem* HitParticles;

	/**
	/*
	/*	Player Stats 
	/* 
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Amimation")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Stats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float RunninSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float SprintingSpeed;

	bool bShiftKeydown;
	bool bLeftMouseButtomDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float StaminaDrainRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats | Movement")
	float MinSprintStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Amimation")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* HitSound;

	//Interpolation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemy* CombatTarget;
	FORCEINLINE void SetCombatTarget(AEnemy* Target){ CombatTarget = Target; }

	//** */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	//** */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	FVector CombatTargetLocation;//Location to place the enemy Widget

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Interpolation controllers*/

	float InterpSpeed;
	bool bInterpToEnemy;
	void SetInterptoEnemy(bool bIntep);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**Called for forwards/backwards inputs*/
	void MoveForward(float input);

	/**Call for sid to side input*/
	void MoveRight(float input);
	
	/** Called Via input to turn at given rate
	* @param rate this is a normalized rate i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float rate);

	/**Called Via input to look Up/Down at given rate
	* @param rate this is a normalized rate i.e. 1.0 means 100% of desired look Up/Down rate
	*/
	void LookUpAtRate(float rate);

	bool bActionPerformed;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return FollowCamera; }

	/**
	*
	* Character properties access
	* 
	*/

	void SetMovementStatus(EMovementStatus eStatus);
	void SetEquippedWeapon(AWeapon* weapToSet);

	FORCEINLINE void SetStaminaStatus(EStaminaStatus eStatus){ StaminaStatus = eStatus; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* item){ ActiveOverlappingItem = item; }

	//Pressed down to enable spriting
	void Sprinting_ShiftKeyDown();
	//Released to stops printing 
	void Running_ShiftKeyUp();

	void DecrementHealth(const float dmg);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCouser)override;

	//increment section for PICK UPS BP
	UFUNCTION(BlueprintCallable)
	void IncrementCoin(const int32 value);
	UFUNCTION(BlueprintCallable)
	void IncrementHealth(const float value);

	void Die();

	UFUNCTION(BlueprintCallable, Category = "Player Stats | Movement")
	void DeathEnd();

	//E key
	void ActionPerformed_E_Pressed();
	void ActionPerformed_E_UP();

	//Left mouse buttom
	void AttackPerformed_LMB_Pressed();
	void AttackPerformed_LMB_UP();

	UFUNCTION(BlueprintCallable, Category = "Player Stats | Movement")
	void Attackfinished();

	virtual void Jump()override;

	//Function to update my combatTarget in game
	void UpdateCombatTarget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;

	void SwitchLevel(const FName LevelName);

private:

	FName GetAttackAnimationName();

	void UpdateStaminaStatus(float& DeltaTime);

	//Interpolation
	FRotator LookAtRotationYaw(FVector Target);
	void ActorFaceEnemy(float DeltaTime);


};