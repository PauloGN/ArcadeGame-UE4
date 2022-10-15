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
