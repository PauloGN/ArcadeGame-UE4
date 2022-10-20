// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorage.generated.h"

//Forward Declararions

class AWeapon;

UCLASS()
class ARCADEACTION_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemStorage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// key --> Value
	UPROPERTY(EditDefaultsOnly, Category = "Weapons | SaveData")
	TMap<FString, TSubclassOf<AWeapon>> WeaponCollecionMAP;

};
