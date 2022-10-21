// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArcadeSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	//properties and objs
	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	float Health;

	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	float MaxHealth;

	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	float Stamina;

	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	float MaxStamina;

	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	int32 Coins;

	//Transform
	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	FVector Location;

	UPROPERTY(VisibleAnyWhere, Category = "SaveGameData | Character")
	FRotator Rotation;
	
	//Items

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGameData | Item")
	FString WeaponName = TEXT("");

	//Level Ref

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGameData | Item")
	FString LevelName = TEXT("");
};

/**
 * 
 */
UCLASS()
class ARCADEACTION_API UArcadeSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UArcadeSaveGame();

	UPROPERTY(VisibleAnyWhere, Category = "Save")
	FString PlayerName;

	UPROPERTY(VisibleAnyWhere, Category = "Save")
	uint32 UserIndex;

	UPROPERTY(VisibleAnyWhere, Category = "Save")
	FCharacterStats CharacterStats;
	
};
