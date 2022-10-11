// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEACTION_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UEnemyAnimInstance();

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class APawn* EnemyPawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class AEnemy* EnemyREF;

};
