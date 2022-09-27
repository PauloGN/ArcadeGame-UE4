// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}
	//MyMovementComponent->AddInputVector(Foraward * value); then reset to zero
	FVector DesiredUpdateThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f);

	if (!DesiredUpdateThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredUpdateThisFrame, UpdatedComponent->GetComponentRotation(),true, Hit);

		//if we bumped with something slide along the side of it
		if (Hit.IsValidBlockingHit())
		{
			GEngine->AddOnScreenDebugMessage(1, 15, FColor::Red, TEXT("Right"));

			SlideAlongSurface(DesiredUpdateThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}

}

