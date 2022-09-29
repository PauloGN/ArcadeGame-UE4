// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEACTION_API APickup : public AItem
{
	GENERATED_BODY()
	
public:

	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins Properties")
	int32 CoinAmount;

	//UFUNCTION() from the parent class
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	//UFUNCTION() from the parent class
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
};
