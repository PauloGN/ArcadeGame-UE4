// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "MainCharacter.h"


void UMainAnimInstance::NativeInitializeAnimation()
{

	if (PawnREF == nullptr)
	{
		PawnREF = TryGetPawnOwner();

		if (PawnREF)
		{
			MainCharacter = Cast<AMainCharacter>(PawnREF);
		}
	}
}


void UMainAnimInstance::UpdateAnimationProperties()
{
	if (PawnREF == nullptr)
	{
		PawnREF = TryGetPawnOwner();
	}

	if (PawnREF)
	{
		FVector Speed = PawnREF->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInTheAir = PawnREF->GetMovementComponent()->IsFalling();

		if (MainCharacter == nullptr)
		{
			MainCharacter = Cast<AMainCharacter>(PawnREF);
		}
	}
}
