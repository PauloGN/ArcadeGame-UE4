// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater() :InitialLoc(FVector(0.0f)), PlacedLoc(FVector(0.0f)), bInitializeFloaterLocations(false), WorldOrigin(FVector(0.0f)), bShouldFloat(false),
	InitialDirection(FVector(0.0f)), InitialForce (FVector(9000000.f, 0.0f, 0.0f)), InitialTorqueForce(FVector(9000000.f, 0.0f, 0.0f)), RunningTime(0.0f),
	BaseZLOC(0.0f), Amplitude(0.0f), Period(0.0f), PhaseShif(0.0f), VerticalShif(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));


}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	PlacedLoc = GetActorLocation();
	BaseZLOC = PlacedLoc.Z;

	float X = FMath::FRand();
	float Y = FMath::FRand();
	float Z = FMath::FRand();

	X = FMath::FRandRange(-500.f,500.f);
	Y = FMath::FRandRange(-500.f,500.f);
	Z = FMath::FRandRange(-10.f,500.f);

	InitialLoc = FVector(X, Y, Z);
	//InitialLoc *= 10.f;


	if (bInitializeFloaterLocations)
	{
		//Set the location to the position of the FVector variable
		SetActorLocation(InitialLoc);
	}

	/*
	//Translation in local and World based
	FHitResult hitResult;
	FVector LocalOfSet = FVector(600.f, 0.0f, 0.0f);
	AddActorLocalOffset(LocalOfSet, true, &hitResult);
	AddActorWorldOffset(LocalOfSet, true, &hitResult);
	*/

	//FRotator Rotation = FRotator(0.0f, 1.0f, 0.0f);
	//AddActorLocalRotation(Rotation);
	//AddActorWorldRotation(Rotation);


	//StaticMesh->AddForce(InitialForce);

	//StaticMesh->AddTorqueInDegrees(InitialTorqueForce);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bShouldFloat)
	//{
	//	FHitResult hitResult;
	//	AddActorLocalOffset(InitialDirection, true, &hitResult);

	//	//FVector HitLocation = hitResult.Location;
	//	//UE_LOG(LogTemp, Warning, TEXT("Location: X: %f  Y: %f Z: %f"), HitLocation.X, HitLocation.Y, HitLocation.Z);

	//}

	if (bShouldFloat)
	{
		FVector NewLocation = GetActorLocation();

		//Sin function gest the time and returns a value between -1 and 1, the base Z location is the base of the height then to make the obj float up and down we multiply by a float number witch is the amplitude
		NewLocation.Z = BaseZLOC + ( Amplitude * FMath::Sin(RunningTime * Period - PhaseShif) + VerticalShif);

		//if (NewLocation.Z < PlacedLoc.Z)
		//{
		//	NewLocation.Z *= -1;
		//}
		SetActorLocation(NewLocation);
		//Acumulate the amount of time since the last frame into Running time variable
		RunningTime += DeltaTime;
	}

}

