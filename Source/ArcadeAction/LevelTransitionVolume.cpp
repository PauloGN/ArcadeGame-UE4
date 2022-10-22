// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "MainCharacter.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Collider to change level
	BoxTransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = BoxTransitionVolume;

	//Collider to save game
	BoxSaveGameVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Save Game Volume"));
	BoxSaveGameVolume->SetupAttachment(GetRootComponent());

	BillBoard = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillBoard"));
	BillBoard->SetupAttachment(GetRootComponent());

	//Level to load after overlapping BoxTransitionVolume
	NextLevel = FName("AdventureMap");
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	BoxSaveGameVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBoxSaveGameVolume);
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharREF = Cast<AMainCharacter>(OtherActor);

		if (MainCharREF)
		{
			MainCharREF->SwitchLevel(NextLevel);
		}
	}
}

void ALevelTransitionVolume::OnOverlapBoxSaveGameVolume(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		AMainCharacter* MainCharREF = Cast<AMainCharacter>(OtherActor);

		if (MainCharREF)
		{
			MainCharREF->SaveGame(NextLevel.ToString());
		}
	}
}