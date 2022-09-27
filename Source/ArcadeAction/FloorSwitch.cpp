// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloorSwitch::AFloorSwitch(): SwitchDelay(2.f), bCharacterStilOnSwitch(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger Box");
	RootComponent = TriggerBox;

	//Trigger box presets
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(62.f,62.f, 32.f));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>("Floor");
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	Door->SetupAttachment(GetRootComponent());

}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterStilOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();	
	}
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialFloorSwitchLocation = FloorSwitch->GetComponentLocation();

}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCharacterStilOnSwitch)
	{
		bCharacterStilOnSwitch = true;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Begin Overlap");
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bCharacterStilOnSwitch)
	{
		bCharacterStilOnSwitch = false;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, "End Overlap");
	GetWorldTimerManager().SetTimer(SwichHandle, this, &ThisClass::CloseDoor,SwitchDelay);
}

void AFloorSwitch::UpdateDoorLocationInZ(float z)
{

	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += z;

	Door->SetWorldLocation(NewLocation);

}

void AFloorSwitch::UpdateFloorLocationInZ(float z)
{

	FVector NewLocation = InitialFloorSwitchLocation;
	NewLocation.Z += z;

	FloorSwitch->SetWorldLocation(NewLocation);

}

