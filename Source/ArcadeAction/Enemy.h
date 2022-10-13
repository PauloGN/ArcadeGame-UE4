// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UENUM(BlueprintType)
 enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		UMETA(DisplayName = "Attacking"),
	EMS_MAX				UMETA(DisplayName = "Default")
};



UCLASS()
class ARCADEACTION_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	//Used to detect players on range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AMainCharacter* CombateTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float MAXHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	float DamagePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* AttackSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	class UBoxComponent* BoxCombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	bool bAttacking;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetEnemyMovementStatus( EEnemyMovementStatus EnemyMoveStatus){ EnemyMovementStatus = EnemyMoveStatus; }
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus( ) const { return EnemyMovementStatus; }

	//The Overlap Functions

	UFUNCTION()
	virtual void AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveToTarget(class AMainCharacter* TargetToFollow);

	UFUNCTION(BlueprintCallable, Category = "AI | Sound")
	void PlaySoundAttack(class AMainCharacter* TargetToFollow);
	
	UFUNCTION()
	void OnCombateBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ActivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DeactivateCollision();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void AttackEnd();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Attack();

private:

	FName GetAttackAnimationName();
};
