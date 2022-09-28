// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform():StartPoint(FVector(0.f)), EndPoint(FVector(0.f)), PlatformSpeed(4.f),
	bInterping(false), PlatformTime(1.f), Distance(0.f)
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

	GetWorldTimerManager().SetTimer(PlatformTimer, this, &ThisClass::ToggleInterping, PlatformTime);
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, PlatformSpeed);
		SetActorLocation(Interp);

		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

		if (Distance - DistanceTraveled <= 0.5f)
		{
			ToggleInterping();
			GetWorldTimerManager().SetTimer(PlatformTimer, this, &ThisClass::ToggleInterping, PlatformTime);
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

