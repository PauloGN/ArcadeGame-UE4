// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this,HUDOverlayAsset);
	}
	else
	{
		return;
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	//Hold the enemy health bar
	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);

		if (EnemyHealthBar)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Controller...................");
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);

			FVector2D AlignmentVec(0.f, 0.f);
			EnemyHealthBar->SetAlignmentInViewport(AlignmentVec);
		}
	}

	//My Menu

	if (WPauseMenu)
	{
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);

		if (PauseMenu)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "Controller...................");
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D ViewPortPosition;
		ProjectWorldLocationToScreen(EnemyLocation, ViewPortPosition);

		ViewPortPosition.Y -= 110.f;

		const FVector2D SizeInViewPort = FVector2D(200.f, 10.f);

		EnemyHealthBar->SetPositionInViewport(ViewPortPosition);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
}


void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
/** MENU */
void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);

		//
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		bPauseMenuVisible = false;
		//
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

bool AMainPlayerController::TogglePauseMenu()
{

	if (bPauseMenuVisible)
	{
		RemovePauseMenu();
	}
	else
	{
		DisplayPauseMenu();
	}
	return bPauseMenuVisible;
}