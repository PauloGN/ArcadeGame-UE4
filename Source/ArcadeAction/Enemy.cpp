// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GamePlayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"



// Sets default values
AEnemy::AEnemy():EnemyMovementStatus(EEnemyMovementStatus::EMS_Idle)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIController = Cast<AAIController>(GetController());

	//Field of view to detect players
	AgroSphere = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	//Range of attack
	CombatSphere = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(100.f);

	//Range to apply damage
	BoxCombatComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCombateComponent"));
	BoxCombatComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("b_MF_Weapon_R_Socket"));

	bOverlappingCombatSphere = false;
	bAttacking = false;
	CombateTarget = nullptr;
	

	HitSound = nullptr;
	CombatMontage = nullptr;
	/** Enemy properties*/

		Health = 75.f ;
		MAXHealth = 100.f;
		DamagePower = 10.f;

		HitParticles = nullptr;
		AttackSound = nullptr;
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


	BoxCombatComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCombateBoxOverlapBegin);
	BoxCombatComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCombatBoxOverlapEnd);

	//**   Setting Up collision
	//not use phisics just generate overlap events
	BoxCombatComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Type of collision for the box something that is moving arroud at the world 
	BoxCombatComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//By Default ignore all channels
	BoxCombatComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Response to Pawn obj and generate overlap events only
	BoxCombatComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

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
			MainChar->SetCombatTarget(this);
			CombateTarget = MainChar;
			bOverlappingCombatSphere = true;
			Attack();
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
			MainChar->SetCombatTarget(nullptr);
			bOverlappingCombatSphere = false;
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			//MoveToTarget(MainChar);
			if (!bAttacking)
			{
				MoveToTarget(MainChar);
				//CombateTarget = nullptr;
			}
			
			EnemyMovementStatus = EEnemyMovementStatus::EMS_MoveToTarget;
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

void AEnemy::PlaySoundAttack(AMainCharacter* TargetToFollow)
{
	if (AttackSound)
	{
		UGameplayStatics::PlaySound2D(this, AttackSound);
	}
}

//Box To Apply damage

void AEnemy::OnCombateBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharREF = Cast<AMainCharacter>(OtherActor);

		if (MainCharREF)
		{
			if (MainCharREF->HitParticles)
			{
				const USkeletalMeshSocket* SpawnSocket = MainCharREF->GetMesh()->GetSocketByName(FName("SpawnSocket"));
				if (SpawnSocket)
				{
					FVector SocketLocation = SpawnSocket->GetSocketLocation(MainCharREF->GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MainCharREF->HitParticles, SocketLocation, FRotator().ZeroRotator, false);
				}
			}

			if (MainCharREF->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, MainCharREF->HitSound);
			}

		}
	}
}

void AEnemy::OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::ActivateCollision()
{
	BoxCombatComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateCollision()
{
	BoxCombatComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	
	if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
	{
		MoveToTarget(CombateTarget);
	}

	if (bOverlappingCombatSphere)
	{
		Attack();
	}
}

void AEnemy::Attack()
{
	if (AIController)
	{
		AIController->StopMovement();
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
	}

	if (!bAttacking)
	{
		bAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		FName AnimationName = GetAttackAnimationName();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(AnimationName, CombatMontage);
		}

		if (AttackSound)
		{
			UGameplayStatics::PlaySound2D(this, AttackSound);
		}

	}
}

//Generate radon attack
FName AEnemy::GetAttackAnimationName()
{

	int32 Selection = FMath::RandRange(0, 1);
	FName AttackName = FName("null");


	switch (Selection)
	{
	case 0:
		AttackName = TEXT("Attack_1");
		break;
	case 1:
		AttackName = TEXT("Attack_2");
		break;

	default:
		;
	}

	return AttackName;

}