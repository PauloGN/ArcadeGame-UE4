// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADEACTION_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Reference to the UMG asset in the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;
	
	/** Variable to hold a widger after been created*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* HUDOverlay;


	/** Reference to the UMG asset in the editor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBarVisible;
	
	FVector EnemyLocation;

protected:


	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();
};
