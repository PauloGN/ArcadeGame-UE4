// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform():StartPoint(FVector(0.f)), EndPoint(FVector(0.f)), InterpolationSpeed(4.f),
	bInterping(false), PlatformStopTime(1.f), Distance(0.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = PlatformMesh;


}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();
	EndPoint += StartPoint;

	//sets a delay before tells the platform to start interping and moving ahead
	GetWorldTimerManager().SetTimer(PlatformTimer, this, &ThisClass::ToggleInterping, PlatformStopTime);
	//Total distance from the origin to the target
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		//Makes the travel smoother from the origin to the target Point baseed on Delta Time and speed of interpolation;
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpolationSpeed);
		//Get the frame position ahead calculeted and set the new Interpolate Location
		SetActorLocation(Interp);

		//Get the Distance traveled to tell the algorithim when it should swap the direction//How far we've traveled
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

		if (Distance - DistanceTraveled <= 0.5f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(PlatformTimer, this, &ThisClass::ToggleInterping, PlatformStopTime);
			//Reverse origin and destination points
			SwapVector(StartPoint, EndPoint);
		}
	}
}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVector(FVector& V1, FVector& V2)
{
	FVector tempV = V1;
	V1 = V2;
	V2 = tempV;
}

