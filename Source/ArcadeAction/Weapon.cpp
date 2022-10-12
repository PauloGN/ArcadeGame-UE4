// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Enemy.h"


AWeapon::AWeapon():OnEquippeSound(nullptr), WeaponState(EWeaponState::EWS_Pickup), Damage(25.f)
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Combat Collision"));
	CombatBoxComponent->SetupAttachment(GetRootComponent());

	//not use phisics just generate overlap events
	CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Type of collision for the box something that is moving arroud at the world 
	CombatBoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//By Default ignore all channels
	CombatBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//Response to Pawn obj and generate overlap events only
	CombatBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AWeapon::BeginPlay()
{

	Super::BeginPlay();


	CombatBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCombateBoxOverlapBegin);
	CombatBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCombatBoxOverlapEnd);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
	{
		AMainCharacter* CharREF = Cast<AMainCharacter>(OtherActor);

		if (CharREF)
		{

			CharREF->SetActiveOverlappingItem(this);
			//Equip(CharREF);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor)
	{
		AMainCharacter* CharREF = Cast<AMainCharacter>(OtherActor);

		if (CharREF)
		{
			CharREF->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::OnCombateBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemy* EnemyREF = Cast<AEnemy>(OtherActor);

		if (EnemyREF)
		{

			if (EnemyREF->HitParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EnemyREF->HitParticles, EnemyREF->GetActorLocation(), FRotator().ZeroRotator, false);
			}
		}
	}
}

void AWeapon::OnCombatBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AWeapon::Equip(AMainCharacter* Char)
{
	if (Char)
	{
		//Dont Get the Camera zooming in if the weapon is in the way
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName(TEXT("RightHandSocket"));
		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());
			bSpinning = false;

			//Set the weapon ref to the char
			Char->SetEquippedWeapon(this);
			Char->SetActiveOverlappingItem(nullptr);
		}
		//Play sound effect
		if (OnEquippeSound && WeaponState == EWeaponState::EWS_Pickup)
		{
			UGameplayStatics::PlaySound2D(this, OnEquippeSound);
			IdleParticleComponent->Deactivate();
			WeaponState = EWeaponState::EWS_Equipped;
		}
	}
}

void AWeapon::ActivateCollision()
{
	CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCollision()
{
	CombatBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
