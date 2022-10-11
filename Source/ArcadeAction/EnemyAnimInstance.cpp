// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

UEnemyAnimInstance::UEnemyAnimInstance(): EnemyPawn(nullptr), EnemyREF(nullptr), MovementSpeed(0.f)
{

	EnemyREF = Cast<AEnemy>(EnemyPawn);

}

void UEnemyAnimInstance::NativeInitializeAnimation()
{

	if (EnemyPawn == nullptr)
	{
		EnemyPawn = TryGetPawnOwner();
		if (EnemyPawn)
		{
			EnemyREF = Cast<AEnemy>(EnemyPawn);
		}
	}


}

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (EnemyPawn == nullptr)
	{
		EnemyPawn = TryGetPawnOwner();
		if (EnemyPawn)
		{
			EnemyREF = Cast<AEnemy>(EnemyPawn);
		}
	}

	if (EnemyPawn)
	{
		FVector Speed = EnemyPawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}


}
