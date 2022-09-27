// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class ARCADEACTION_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere,Category = "Actor Mesh Components")
	UStaticMeshComponent* StaticMesh;

	// Location to set actor location when begin play is called
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialLoc;

	//Location of the actor when dragged in from the editor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector PlacedLoc;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Floater Variables")
	FVector WorldOrigin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	FVector InitialDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bInitializeFloaterLocations;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialForce;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialTorqueForce;

	float RunningTime;
	float BaseZLOC;

	//Amplitude - how much we oscillate up and down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float Amplitude;
	//Period - is 2 * Pi / ABS(Period) - how fast is the period of going up and down 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float Period;
	//Phase shif - is (Vertical / Phase) controls the statirn and ending point of the phase
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float PhaseShif;
	//Vertical shif - is where is the base point where the obj will oscillate.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floater Variables")
	float VerticalShif;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
