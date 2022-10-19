// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>("Spawning Box");

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetSpawnPoint()
{

	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector OriginCenter = SpawningBox->GetComponentLocation();

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(OriginCenter, Extent);

	return Point;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{

	if (ActorsCollection.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, ActorsCollection.Num() - 1);

		return ActorsCollection[Index];
	}

	return nullptr;
}

void ASpawnVolume::SpawnActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		if (World)
		{
			AActor* Actor =  World->SpawnActor<AActor>(ToSpawn, Location, FRotator().ZeroRotator, SpawnParams);

			AEnemy* Enemy = Cast<AEnemy>(Actor);

			if (Enemy)
			{
				Enemy->SpawnDefaultController();
				AAIController* Controller = Cast<AAIController>(Enemy->GetController());
				if (Controller)
				{
					Enemy->AIController = Controller;
				}
			}

		}
	}

}

