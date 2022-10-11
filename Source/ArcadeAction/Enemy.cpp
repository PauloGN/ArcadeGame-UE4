// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemy::AEnemy():EnemyMovementStatus(EEnemyMovementStatus::EMS_Idle)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIController = Cast<AAIController>(GetController());

	AgroSphere = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);

	bOverlappingCombatSphere = false;

	CombateTarget = nullptr;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AIController == nullptr)
	{
		AIController = Cast<AAIController>(GetController());
	}

	//Bind Functions to the Overlap Events
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AgroOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::AgroOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::CombatOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::CombatOnOverlapEnd);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/** THE OVERLAP FUNCTIONS  */

void AEnemy::AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor)
	{
		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

		if (MainChar)
		{
			MoveToTarget(MainChar);
		}
	}


}

void AEnemy::AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

		if (MainChar)
		{
			if (AIController)
			{
				AIController->StopMovement();
			}
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
		}
	}
}

//Combat Sphere
void AEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);
		
		if (MainChar)
		{
			CombateTarget = MainChar;
			bOverlappingCombatSphere = true;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}

}

void AEnemy::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* MainChar = Cast<AMainCharacter>(OtherActor);

		if (MainChar)
		{
			bOverlappingCombatSphere = false;
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			//MoveToTarget(MainChar);
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(MainChar);
				//CombateTarget = nullptr;
			}

		}
	}
}

void AEnemy::MoveToTarget(AMainCharacter* Target)
{

	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		//Distance between char capsule and Enemy capsule
		MoveRequest.SetAcceptanceRadius(50.0f);

		//Variable will be filled in with importantes informations such as Path points
		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);

		TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();

		/*
		for (FNavPathPoint& point: PathPoints)
		{
			FVector Location = point.Location;

			UKismetSystemLibrary::DrawDebugSphere(this, Location + FVector(0.f, 0.f, 100.f), 15.f, 12, FColor::Blue, 5.f, 2.f);

		}
		*/

	}

}

